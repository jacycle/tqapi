#include <sstream>
#include <unordered_set>
#include "tquant_api.h"
#include "myutils/stringutils.h"
#include "myutils/jsonrpc.h"
#include "data_api_impl.h"
#include "trade_api_impl.h"

namespace tquant { namespace api { namespace impl {

    using namespace ::jsonrpc;
    using namespace ::tquant::api;

    class DataApiImpl;
    class TradeApiImpl;

    static inline bool is_nil(msgpack_object& obj)
    {
        return obj.type == MSGPACK_OBJECT_NIL;
    }

    static inline bool is_bin(msgpack_object& obj)
    {
        return obj.type == MSGPACK_OBJECT_BIN;
    }

    static string builld_errmsg(int code, const string& msg)
    {
        stringstream ss;
        ss << code << "," << msg;
        return ss.str();
    }

    struct BinDataHead {
        uint32_t element_size;
        uint32_t element_count;
        char     data[];

        template<typename T> T* elements() const {
            return reinterpret_cast<T*>(data);
        }
    };

    class DataApiImpl : public DataApi {
        JsonRpcClient*        m_client;
        unordered_set<string> m_sub_codes;
        uint64_t              m_sub_hash;
        DataApi_Callback*     m_callback;
    public:
        DataApiImpl(JsonRpcClient* client) 
            : m_client(client)
            , m_sub_hash(0)
            , m_callback(nullptr)
        {}

        virtual ~DataApiImpl() override
        {}

        virtual CallResult<vector<MarketQuote>> tick(const char* code, int trading_day) override
        {
            MsgPackPacker pk;
            pk.pack_map(3);
            pk.pack_map_item("code", code);
            pk.pack_map_item("trading_day", trading_day);
            pk.pack_map_item("_format", "bin");

            auto rsp = m_client->call("dapi.tst", pk.sb.data, pk.sb.size);
            if (!is_bin(rsp->result))
                return CallResult<vector<MarketQuote>>(builld_errmsg(rsp->err_code, rsp->err_msg));
            
            const BinDataHead* bin_head = reinterpret_cast<const BinDataHead*>(rsp->result.via.bin.ptr);
            uint32_t bin_len = rsp->result.via.bin.size;

            if (bin_head->element_size < sizeof(RawMarketQuote))
                return CallResult<vector<MarketQuote>>("-1,wrong data format");
            auto ticks = make_shared<vector<MarketQuote>>();
            const char* p = bin_head->data;
            for (uint32_t i = 0; i < bin_head->element_count; i++) {
                ticks->push_back(MarketQuote(*reinterpret_cast<const RawMarketQuote*>(p), code));
                p += bin_head->element_size;
            }
            return CallResult<vector<MarketQuote>>(ticks);
        }

        virtual CallResult<vector<Bar>> bar(const char* code, const char* cycle, int trading_day, bool align)
        {
            MsgPackPacker pk;
            pk.pack_map(5);
            pk.pack_map_item("code",        code);
            pk.pack_map_item("cycle",       cycle);
            pk.pack_map_item("trading_day", trading_day);
            pk.pack_map_item("align",       align);
            pk.pack_map_item("_format",     "bin");

            auto rsp = m_client->call("dapi.tsi", pk.sb.data, pk.sb.size);
            if (!is_bin(rsp->result))
                return CallResult<vector<Bar>>(builld_errmsg(rsp->err_code, rsp->err_msg));

            const BinDataHead* bin_head = reinterpret_cast<const BinDataHead*>(rsp->result.via.bin.ptr);
            uint32_t bin_len = rsp->result.via.bin.size;

            if (bin_head->element_size < sizeof(RawBar))
                return CallResult<vector<Bar>>("-1,wrong data format");
            auto bars = make_shared<vector<Bar>>();
            const char* p = bin_head->data;
            for (uint32_t i = 0; i < bin_head->element_count; i++) {
                bars->push_back(Bar(*reinterpret_cast<const RawBar*>(p), code));
                p += bin_head->element_size;
            }
            return CallResult<vector<Bar>>(bars);
        }

        virtual CallResult<vector<DailyBar>> daily_bar(const char* code, const char* price_adj, bool align) override
        {
            MsgPackPacker pk;
            pk.pack_map(5);
            pk.pack_map_item("code",        code);
            pk.pack_map_item("cycle",       "1d");
            pk.pack_map_item("price_adj",   price_adj);
            pk.pack_map_item("align",       align);
            pk.pack_map_item("_format",     "bin");

            auto rsp = m_client->call("dapi.tsi", pk.sb.data, pk.sb.size);
            if (!is_bin(rsp->result))
                return CallResult<vector<DailyBar>>(builld_errmsg(rsp->err_code, rsp->err_msg));

            const BinDataHead* bin_head = reinterpret_cast<const BinDataHead*>(rsp->result.via.bin.ptr);
            uint32_t bin_len = rsp->result.via.bin.size;

            int i = sizeof(DailyBar);

            if (bin_head->element_size < sizeof(RawDailyBar))
                return CallResult<vector<DailyBar>>("-1,wrong data format");

            auto bars = make_shared<vector<DailyBar>>();
            const char* p = bin_head->data;
            for (uint32_t i = 0; i < bin_head->element_count; i++) {
                bars->push_back(DailyBar(*reinterpret_cast<const RawDailyBar*>(p), code));
                p += bin_head->element_size;
            }
            return CallResult<vector<DailyBar>>(bars);
        }

        virtual CallResult<MarketQuote> quote(const char* code) override
        {
            MsgPackPacker pk;
            pk.pack_map(2);
            pk.pack_map_item("code", code);
            pk.pack_map_item("_format", "bin");

            auto rsp = m_client->call("dapi.tsq_quote", pk.sb.data, pk.sb.size);
            if (!is_bin(rsp->result))
                return CallResult<MarketQuote>(builld_errmsg(rsp->err_code, rsp->err_msg));

            const BinDataHead* bin_head = reinterpret_cast<const BinDataHead*>(rsp->result.via.bin.ptr);
            uint32_t bin_len = rsp->result.via.bin.size;

            if (bin_head->element_size < sizeof(MarketQuote))
                return CallResult<MarketQuote>("-1,wrong data format");
            auto quote = make_shared<MarketQuote>(*bin_head->elements<const MarketQuote>());
            return CallResult<MarketQuote>(quote);
        }

        virtual CallResult<vector<string>> subscribe(const vector<string>& codes) override
        {
            stringstream ss;
            for (int i = 0; i < codes.size(); i++) {
                if (i != codes.size() - 1)
                    ss << codes[i] << ",";
                else
                    ss << codes[i];
            }

            MsgPackPacker pk;
            pk.pack_map(2);
            pk.pack_map_item ("codes",        ss.str());
            pk.pack_map_item ("want_bin_fmt", true);

            auto rsp = m_client->call("dapi.tsq_sub", pk.sb.data, pk.sb.size);
            if (is_nil(rsp->result))
                return CallResult<vector<string>>(builld_errmsg(rsp->err_code, rsp->err_msg));

            if (!is_map(rsp->result))
                return CallResult<vector<string>>("-1,wrong data format");
            
            string new_codes;
            get_map_field_int(rsp->result, "sub_hash", (int64_t*)&m_sub_hash);
            get_map_field_str(rsp->result, "sub_codes", &new_codes);

            vector<string> sub_codes;
            split(new_codes, ",", &sub_codes);

            m_sub_codes.clear();
            for (auto& s : sub_codes) m_sub_codes.insert(s);

            return CallResult<vector<string>>(make_shared <vector<string>>(sub_codes));
        }

        virtual CallResult<vector<string>> unsubscribe(const vector<string>& codes) override
        {
            MsgPackPacker pk_codes;
            pk_codes.pack_array(codes.size());
            for (auto & code : codes) {
                pk_codes.pack_string(code);
                auto it = m_sub_codes.find(code);
                if (it != m_sub_codes.end())
                    m_sub_codes.erase(it);
            }

            stringstream ss;
            for (int i = 0; i < codes.size(); i++) {
                if (i != codes.size() - 1)
                    ss << codes[i] << ",";
                else
                    ss << codes[i];
            }

            MsgPackPacker pk;
            pk.pack_map(2);
            pk.pack_map_item("codes",        ss.str());
            pk.pack_map_item("want_bin_fmt", true);

            auto rsp = m_client->call("dapi.tsq_unsub", pk.sb.data, pk.sb.size);
            if (is_nil(rsp->result))
                return CallResult<vector<string>>(builld_errmsg(rsp->err_code, rsp->err_msg));

            if (!is_map(rsp->result))
                return CallResult<vector<string>>("-1,wrong data format");

            string new_codes;
            get_map_field_int(rsp->result, "sub_hash", (int64_t*)&m_sub_hash);
            get_map_field_str(rsp->result, "sub_codes", &new_codes);

            vector<string> sub_codes;
            split(new_codes, ",", &sub_codes);

            m_sub_codes.clear();
            for (auto& s : sub_codes) m_sub_codes.insert(s);

            return CallResult<vector<string>>(make_shared <vector<string>>(sub_codes));
        }

        virtual void setCallback(DataApi_Callback* callback) override
        {
            m_callback = callback;
        }

        void on_notification(shared_ptr<JsonRpcMessage> rpcmsg)
        {
            if (!m_callback) return;

            if (rpcmsg->method == "dapi.quote") {
                if (!is_bin(rpcmsg->params)) return;
                const char* code = rpcmsg->params.via.bin.ptr;
                auto quote = make_shared<MarketQuote>(*(RawMarketQuote*)(code + strlen(code) + 1), code);
                m_callback->onMarketQuote(quote);
            }
            else if (rpcmsg->method == "dapi.bar") {
                if (!is_bin(rpcmsg->params)) return;
                const char* cycle = rpcmsg->params.via.bin.ptr;
                const char* code = cycle + strlen(cycle) + 1;
                const RawBar* rbar = reinterpret_cast<const RawBar*>(code + strlen(code) + 1);

                auto bar = make_shared<Bar>(*rbar, code);
                m_callback->onBar(cycle, bar);
            }
        }
    };

    class TradeApiImpl : public TradeApi {
        JsonRpcClient* m_client;
    public:
        TradeApiImpl(JsonRpcClient* client)
            : m_client(client)

        {}

        virtual ~TradeApiImpl() override
        {}

        void on_notification(shared_ptr<JsonRpcMessage> rpcmsg) {
        }
    };

    class TQuantApiImpl : public TQuantApi, public JsonRpcClient_Callback {
        friend DataApiImpl;
        friend TradeApiImpl;
    public:
        TQuantApiImpl(const char* addr) {
            m_client = new JsonRpcClient();
            m_client->connect(addr, this);
            m_dapi = new DataApiImpl(this->m_client);
            m_tapi = new TradeApiImpl(this->m_client);
        }

        virtual ~TQuantApiImpl() override {
            delete m_dapi;
            delete m_tapi;
            delete m_client;
        }

        virtual TradeApi* trade_api() { return m_tapi; }

        virtual DataApi*  data_api()  { return m_dapi; }

        virtual void on_connected() {}

        virtual void on_disconnected() {}

        virtual void on_notification(shared_ptr<JsonRpcMessage> rpcmsg) {
            if (strncmp(rpcmsg->method.c_str(), "dapi.", 5) == 0) {
                m_dapi->on_notification(rpcmsg);
            }
            else if (strncmp(rpcmsg->method.c_str(), "tapi.", 5) == 0) {
                m_tapi->on_notification(rpcmsg);
            }
            else  {
                m_dapi->on_notification(rpcmsg);
                m_tapi->on_notification(rpcmsg);
            }
        }
        virtual void on_call_result(int callid, shared_ptr<JsonRpcMessage> cr) {}
    private:
        JsonRpcClient*  m_client;
        DataApiImpl*    m_dapi;
        TradeApiImpl*   m_tapi;
    };

} } }

namespace tquant { namespace api {

    TQuantApi* TQuantApi::create(const char* addr)
    {
        return new impl::TQuantApiImpl(addr);
    }

} }