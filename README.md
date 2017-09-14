# tqc-api

tqc-api是量化交易平台TQuant的客户端API。它提供统一的行情接口和交易接口，用于期货、股票的交易。

TQuant为自用的交易平台。它从各互联网网站抓取行情，整理后推送给客户端，并提供历史数据服务。

## 特点

* 推送行情
* 快速获取历史数据，包括 tick, 分钟线，日线
* 交易接口统一，同时支持期货和股票
* 订单状态、成交回报实时通知
* 支持多种编程语言
* 支持 Windows, Linux, OSX三种平台，在Mac上也可以进行交易和研究
* 客户端源码开放，用户策略安全可靠

## 行情接口

提供国内六大交易所的股票、指数、基金、商品期货、股指期货等行情。

**接口列表**

| 函数            | 功能                |
| ------------- | ----------------- |
| quote()       | 取最新行情             |
| bar()         | 取K线，支持实时和历史K线     |
| tick()        | 取tick数据，支持实时和历史数据 |
| subscribe()   | 订阅行情              |
| unsubscribe() | 取消订阅              |


## 交易接口

支持CTP交易通道，支持国内部分券商交易通道。

**接口列表**

| 函数                | 功能                |
| ----------------- | ----------------- |
| query_balance()   | 取资金信息             |
| query_orders()    | 取当日订单列表           |
| query_trades()    | 取当日成交列表           |
| query_positions() | 取当日持仓             |
| place_order()     | 下单                |
| cancel_order()    | 取消订单              |
| query()           | 通用查询接口，可以查询代码表等信息 |


## 编程语言
tqc-api支持三种编程语言，Python, Java和Scala。使用说明参见源码注释和Wiki。


## 注意
* **使用本接口，用户需要运行本地的服务程序 tqc。该服务负责用户登录、数据加速，并提供tqc-api服务。**
* tqc下载地址： [TQuant](http://tquant.oicp.net/tqc)。目前只发布了Windows版本。
* 请添加微信号：tquant, 获得试用帐号。
* 历史数据期限

| 数据类型 | 期限   |
| ---- | ---- |
| tick | 3个月  |
| 分钟线  | 1 年  |
| 日行情  | 全部   |

