
package com.test.sdk;

public interface Constants {
    /**
     * MONEY AMOUNT以分为单位。100分，即1元。MONEY AMOUNT大于等于100时，即定额支付。
     */
    public static final String DEMO_FIXED_PAY_MONEY_AMOUNT = "1";

    /**
     * MONEY AMOUNT为0分时，即不定额支付。
     */
    public static final String DEMO_NOT_FIXED_PAY_MONEY_AMOUNT = "0";

    /**
     * 人民币与游戏充值币的比例，例如2，代表1元人民币可以兑换2个游戏币，整数。
     */
    public static final String DEMO_PAY_EXCHANGE_RATE = "1";

    /**
     * 购买商品的商品id，应用指定，最大16字符。
     */
    public static final String DEMO_PAY_PRODUCT_ID = "100";

    /**
     * 应用内的用户id。 最大32字符。
     */
    public static final String DEMO_PAY_APP_USER_ID = "1888";

    /**
     * 应用服务器为360服务器提供的支付结果通知接口，由360服务器把支付结果通知到这个URI。URI最大255字符。具体协议请查看文档中，
     * 支付结果通知接口。 (这是DEMO专用的URL，请使用方自己搭建自己的应用服务器，此服务器只认DEMO的AppKey。)
     */
    public static final String DEMO_APP_SERVER_NOTIFY_URI = "http://sdbxapp.msdk.mobilem.360.cn/pay_callback.php";

    /**
     * 应用服务器为应用客户端提供的接口Url，用于通过AuthorizationCode获取TokenInfo
     * (这是DEMO专用的URL，请使用方自己搭建自己的应用服务器，此服务器只认DEMO的AppKey。)
     */
    public static final String DEMO_APP_SERVER_URL_GET_TOKEN = "http://sdbxapp.msdk.mobilem.360.cn/mobileSDK/api.php?type=get_token_by_code&debug=1&code=";

    /**
     * 应用服务器为应用客户端提供的接口Url，用于通过AccessToken获取QihooUserInfo
     * (这是DEMO专用的URL，请使用方自己搭建自己的应用服务器，此服务器只认DEMO的AppKey。)
     */
    public static final String DEMO_APP_SERVER_URL_GET_USER = "http://sdbxapp.msdk.mobilem.360.cn/mobileSDK/api.php?type=get_userinfo_by_token&debug=1&token=";

    public static final String IS_LANDSCAPE = "is_landscape";

    public static final String TOKEN_INFO = "token_info";

    public static final String QIHOO_USER_INFO = "qihoo_user_info";

}
