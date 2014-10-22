/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import com.mitsui.game.R;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.qihoo.gamecenter.sdk.common.IDispatcherCallback;
import com.qihoo.gamecenter.sdk.protocols.pay.ProtocolConfigs;
import com.qihoo.gamecenter.sdk.protocols.pay.ProtocolKeys;
import com.qihoopay.insdk.activity.ContainerActivity;
import com.qihoopay.insdk.matrix.Matrix;
import com.test.sdk.Constants;
import com.test.sdk.activity.SdkUserBaseActivity;
import com.test.sdk.appserver.QihooUserInfo;
import com.test.sdk.appserver.QihooUserInfoListener;
import com.test.sdk.appserver.QihooUserInfoTask;
import com.test.sdk.appserver.TokenInfo;
import com.test.sdk.appserver.TokenInfoListener;
import com.test.sdk.appserver.TokenInfoTask;
import com.test.sdk.common.QihooPayInfo;
import com.test.sdk.common.SdkAccountListener;
import com.test.sdk.utils.ProgressUtil;

public class AppActivity extends Cocos2dxActivity implements
		SdkAccountListener, TokenInfoListener, QihooUserInfoListener {

	public static native void startGameScene();

	public static native void ReStartGame();

	private static final String TAG = "AppActivity";
	private static Handler handler;
	// private static Handler SdkPayHandler;
	private TokenInfo mTokenInfo;
	private QihooUserInfo mQihooUserInfo;
	private TokenInfoTask mTokenTask;
	private QihooUserInfoTask mUserInfoTask;

	// for cocos2dx to use
	private static AppActivity instance = new AppActivity();

	public static Object getObj() {
		return instance;
	}

	public void createAdView() {

		Message msg = handler.obtainMessage();
		msg.what = 1;
		handler.sendMessage(msg);

	}

	public void createPayView() {

		Message msg = handler.obtainMessage();
		msg.what = 2;
		handler.sendMessage(msg);

	}

	/*
	 * public void login() {
	 * 
	 * doSdkLogin(false, true); }
	 */

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// 在父类中率先实现了AppDelegate
		super.onCreate(savedInstanceState);
		Matrix.init(this, false, new IDispatcherCallback() {
			@Override
			public void onFinished(String data) {
				// TODO your job
				Log.d(TAG, "matrix startup callback,result is " + data);
			}
		});
		handler = new Handler() {

			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);

				/*
				 * if(msg.arg1==1) { //登陆 doSdkLogin(false,true);
				 * 
				 * } else if(msg.arg2==2) { //付费 doSdkPay(false, false, true); }
				 */
				switch (msg.what) {

				case 1:
					// 先登陆
					doSdkLogin(false, true);
					break;

				case 2:

					// 再付费
					Log.d("x_________", "lallalalalal");
					doSdkPay(false, false, false);
					break;

				}

			}
		};
	}

	/*
	 * handler = new Handler() {
	 * 
	 * @Override public void handleMessage(Message msg) { // TODO Auto-generated
	 * method stub super.handleMessage(msg);
	 * 
	 * switch (msg.what) {
	 * 
	 * case 1: //登陆 doSdkPay(false, false, true); break;
	 * 
	 * case 0:
	 * 
	 * 
	 * break;
	 * 
	 * }
	 * 
	 * } }; // doSdkLogin(false,true); }
	 */

	protected void doSdkLogin(boolean isLandScape, boolean isBgTransparent) {

		Intent intent = getLoginIntent(isLandScape, isBgTransparent);

		Matrix.invokeActivity(this, intent, mLoginCallback);
	}

	/***
	 * * 生成调用360SDK登录接口的Intent * * @param isLandScape 是否横屏 * @param
	 * isBgTransparent 是否背景透明 * @return Intent
	 */
	// private Intent getLoginIntent(boolean isLandScape, boolean
	// isBgTransparent) {
	// return getLoginIntent(isLandScape, isBgTransparent);
	// }

	/***
	 * 生成调用360SDK登录接口的Intent
	 * 
	 * @param isLandScape
	 *            是否横屏
	 * @param isBgTransparent
	 *            是否背景透明
	 * @param appKey
	 *            应用或游戏的AppKey
	 * @param appChannel
	 *            应用或游戏的自定义子渠道
	 * @return Intent
	 */
	private Intent getLoginIntent(boolean isLandScape, boolean isBgTransparent) {
		Bundle bundle = new Bundle(); // 界面相关参数，360SDK界面是否以横屏显示。
		bundle.putBoolean(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE,
				isLandScape);
		// 界面相关参数，360SDK登录界面背景是否透明。
		bundle.putBoolean(ProtocolKeys.IS_LOGIN_BG_TRANSPARENT, isBgTransparent);
		// *** 以下非界面相关参数 ***
		// 必需参数，使用360SDK的登录模块。
		bundle.putInt(ProtocolKeys.FUNCTION_CODE,
				ProtocolConfigs.FUNC_CODE_LOGIN);
		Intent intent = new Intent(this, ContainerActivity.class);
		intent.putExtras(bundle);
		return intent;
	}

	// 登录、注册的回调
	private IDispatcherCallback mLoginCallback = new IDispatcherCallback() {
		@Override
		public void onFinished(String data) {
			Log.d(TAG, "mLoginCallback, data is " + data);

			String authorizationCode = parseAuthorizationCode(data);
			;
			onGotAuthorizationCode(authorizationCode);

		}
	};

	/**
	 * 从Json字符中获取授权码
	 * 
	 * @param data
	 *            Json字符串
	 * @return 授权码
	 */
	private String parseAuthorizationCode(String data) {
		String authorizationCode = null;
		if (!TextUtils.isEmpty(data)) {
			boolean isCallbackParseOk = false;
			try {
				JSONObject json = new JSONObject(data);
				int errCode = json.getInt("errno");
				if (errCode == 0) {
					// 只支持code登陆模式
					JSONObject content = json.getJSONObject("data");
					if (content != null) {
						// 360SDK登录返回的Authorization Code（授权码，60秒有效）。
						authorizationCode = content.getString("code");
						isCallbackParseOk = true;
					}
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}

			// // 用于测试数据格式是否异常。
			// if (!isCallbackParseOk) {
			// Toast.makeText(SdkUserBaseActivity.this,
			// getString(R.string.data_format_error),
			// Toast.LENGTH_LONG).show();
			// }
		}
		Log.d(TAG, "parseAuthorizationCode=" + authorizationCode);
		return authorizationCode;
	}

	private static final String AUTH_CODE = "code";

	@Override
	public void onGotAuthorizationCode(String code) {

		if (TextUtils.isEmpty(code)) {
            Toast.makeText(this, "Got AuthorizationCode failed", Toast.LENGTH_LONG).show();
        } else {
            //clearLoginResult();
            mTokenTask = TokenInfoTask.newInstance();
            /*// 提示用户进度
            mProgress = ProgressUtil.show(this, R.string.get_token_title,
                    R.string.get_token_message, new OnCancelListener() {

                        @Override
                        public void onCancel(DialogInterface dialog) {
                            if (mTokenTask != null) {
                                mTokenTask.doCancel();
                            }
                        }
                    });*/

            // 请求应用服务器，用AuthorizationCode换取AccessToken
            mTokenTask.doRequest(this, code, Matrix.getAppKey(this), this);
        }

	}

	@Override
	public void onGotError(int errCode) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onGotUserInfo(QihooUserInfo userInfo) {
		 if (userInfo != null && userInfo.isValid()) {
	            // 保存QihooUserInfo
	            mQihooUserInfo = userInfo;

	            // 界面显示QihooUser的Id和Name
	            //updateUserInfoUi();
	            Intent intent = getIntent();
	            intent.putExtra(Constants.TOKEN_INFO, mTokenInfo.toJsonString());
	            intent.putExtra(Constants.QIHOO_USER_INFO, mQihooUserInfo.toJsonString());

	        } else {
	            Toast.makeText(this, "get userinfo failed", Toast.LENGTH_LONG).show();
	        }
///////////
		if (userInfo != null && userInfo.isValid()) {
			Toast.makeText(this, "登录成功", Toast.LENGTH_LONG).show();
			startGameScene();
		} else {
			Toast.makeText(this, "登录失败", Toast.LENGTH_LONG).show();
		}

	}

	@Override
	public void onGotTokenInfo(TokenInfo tokenInfo) {

		 if (tokenInfo == null || TextUtils.isEmpty(tokenInfo.getAccessToken())) {
	            //ProgressUtil.dismiss(mProgress);
	            Toast.makeText(this, "未从应用服务器获取Access Token", Toast.LENGTH_LONG).show();
	        } else {
	            // 保存TokenInfo
	            mTokenInfo = tokenInfo;
	            mUserInfoTask = QihooUserInfoTask.newInstance();
	            // 界面显示AccessToken
	            //updateUserInfoUi();

	            /*// 提示用户进度
	            ProgressUtil.setText(mProgress, getString(R.string.get_user_title),
	                    getString(R.string.get_user_message), new OnCancelListener() {

	                        @Override
	                        public void onCancel(DialogInterface dialog) {
	                            if (mUserInfoTask != null) {
	                                mUserInfoTask.doCancel();
	                            }
	                        }
	                    });*/

	            // 请求应用服务器，用AccessToken换取UserInfo
	            mUserInfoTask.doRequest(this, tokenInfo.getAccessToken(), Matrix.getAppKey(this),
	                    this);
	        }
	}

	/**
	 * 使用360SDK的支付接口
	 * 
	 * @param isLandScape
	 *            是否横屏显示支付界面
	 * @param isFixed
	 *            是否定额支付
	 */
	protected void doSdkPay(final boolean isLandScape, final boolean isFixed,
			final boolean isBgTransparent) {

		// 支付基础参数
		Intent intent = getPayIntent(isLandScape, isFixed);

		// 必需参数，使用360SDK的支付模块。
		intent.putExtra(ProtocolKeys.FUNCTION_CODE,
				ProtocolConfigs.FUNC_CODE_PAY);

		// 界面相关参数，360SDK登录界面背景是否透明。
		intent.putExtra(ProtocolKeys.IS_LOGIN_BG_TRANSPARENT, isBgTransparent);

		Matrix.invokeActivity(this, intent, mPayCallback);
	}

	/**
	 * 钩子方法，留给使用支付的子类实现getQihooPayInfo
	 * 
	 * @param isFixed
	 * @return
	 */
	protected QihooPayInfo getQihooPayInfo(boolean isFixed) {
		QihooPayInfo payInfo = null;
        if (isFixed) {
            payInfo = getQihooPay(Constants.DEMO_FIXED_PAY_MONEY_AMOUNT);
        }
        else {
            payInfo = getQihooPay(Constants.DEMO_NOT_FIXED_PAY_MONEY_AMOUNT);
        }

        return payInfo;
	}
	
	/***
     * @param moneyAmount 金额数，使用者可以自由设定数额。金额数为100的整数倍，360SDK运行定额支付流程；
     *            金额数为0，360SDK运行不定额支付流程。
     * @return QihooPay
     */
    private QihooPayInfo getQihooPay(String moneyAmount) {
        // 创建QihooPay
        QihooPayInfo qihooPay = new QihooPayInfo();

        // 登录得到AccessToken和UserId，用于支付。
        String accessToken = (mTokenInfo != null) ? mTokenInfo.getAccessToken() : null;
        String qihooUserId = (mQihooUserInfo != null) ? mQihooUserInfo.getId() : null;

        qihooPay.setAccessToken(accessToken);
        qihooPay.setQihooUserId(qihooUserId);

        qihooPay.setMoneyAmount(moneyAmount);
        qihooPay.setExchangeRate(Constants.DEMO_PAY_EXCHANGE_RATE);

        qihooPay.setProductName(getString(R.string.demo_pay_product_name));
        qihooPay.setProductId(Constants.DEMO_PAY_PRODUCT_ID);

        qihooPay.setNotifyUri(Constants.DEMO_APP_SERVER_NOTIFY_URI);

        qihooPay.setAppName(getString(R.string.demo_pay_app_name));
        qihooPay.setAppUserName(getString(R.string.demo_pay_app_user_name));
        qihooPay.setAppUserId(Constants.DEMO_PAY_APP_USER_ID);

        // 可选参数
        qihooPay.setAppExt1(getString(R.string.demo_pay_app_ext1));
        qihooPay.setAppExt2(getString(R.string.demo_pay_app_ext2));
        qihooPay.setAppOrderId("");

        return qihooPay;
    }

	/***
	 * 生成调用360SDK支付接口基础参数的Intent
	 * 
	 * @param isLandScape
	 * @param pay
	 * @return Intent
	 */
	protected Intent getPayIntent(boolean isLandScape, boolean isFixed) {

		Bundle bundle = new Bundle();

		QihooPayInfo pay = getQihooPayInfo(isFixed);

		// 界面相关参数，360SDK界面是否以横屏显示。
		bundle.putBoolean(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE,
				isLandScape);

		// *** 以下非界面相关参数 ***

		// 设置QihooPay中的参数。
		// 必需参数，用户access token，要使用注意过期和刷新问题，最大64字符。
		bundle.putString(ProtocolKeys.ACCESS_TOKEN, pay.getAccessToken());

		// 必需参数，360账号id，整数。
		bundle.putString(ProtocolKeys.QIHOO_USER_ID, pay.getQihooUserId());

		// 必需参数，所购买商品金额, 以分为单位。金额大于等于100分，360SDK运行定额支付流程； 金额数为0，360SDK运行不定额支付流程。
		bundle.putString(ProtocolKeys.AMOUNT, pay.getMoneyAmount());

		// 必需参数，人民币与游戏充值币的默认比例，例如2，代表1元人民币可以兑换2个游戏币，整数。
		bundle.putString(ProtocolKeys.RATE, pay.getExchangeRate());

		// 必需参数，所购买商品名称，应用指定，建议中文，最大10个中文字。
		bundle.putString(ProtocolKeys.PRODUCT_NAME, pay.getProductName());

		// 必需参数，购买商品的商品id，应用指定，最大16字符。
		bundle.putString(ProtocolKeys.PRODUCT_ID, pay.getProductId());

		// 必需参数，应用方提供的支付结果通知uri，最大255字符。360服务器将把支付接口回调给该uri，具体协议请查看文档中，支付结果通知接口–应用服务器提供接口。
		bundle.putString(ProtocolKeys.NOTIFY_URI, pay.getNotifyUri());

		// 必需参数，游戏或应用名称，最大16中文字。
		bundle.putString(ProtocolKeys.APP_NAME, pay.getAppName());

		// 必需参数，应用内的用户名，如游戏角色名。 若应用内绑定360账号和应用账号，则可用360用户名，最大16中文字。（充值不分区服，
		// 充到统一的用户账户，各区服角色均可使用）。
		bundle.putString(ProtocolKeys.APP_USER_NAME, pay.getAppUserName());

		// 必需参数，应用内的用户id。
		// 若应用内绑定360账号和应用账号，充值不分区服，充到统一的用户账户，各区服角色均可使用，则可用360用户ID最大32字符。
		bundle.putString(ProtocolKeys.APP_USER_ID, pay.getAppUserId());

		// 可选参数，应用扩展信息1，原样返回，最大255字符。
		bundle.putString(ProtocolKeys.APP_EXT_1, pay.getAppExt1());

		// 可选参数，应用扩展信息2，原样返回，最大255字符。
		bundle.putString(ProtocolKeys.APP_EXT_2, pay.getAppExt2());

		// 必选参数，应用订单号，应用内必须唯一，最大32字符。
		bundle.putString(ProtocolKeys.APP_ORDER_ID, pay.getAppOrderId());

		Intent intent = new Intent(this, ContainerActivity.class);
		intent.putExtras(bundle);

		return intent;
	}

	// 支付的回调
	protected IDispatcherCallback mPayCallback = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
			Log.d(TAG, "mPayCallback, data is " + data);
			boolean isCallbackParseOk = false;
			JSONObject jsonRes;
			try {
				jsonRes = new JSONObject(data);
				// error_code 状态码： 0 支付成功， -1 支付取消， 1 支付失败， -2 支付进行中。
				// error_msg 状态描述
				int errorCode = jsonRes.getInt("error_code");
				switch (errorCode) {
				case 0:
					Log.d("x_________", "0");
					ReStartGame();
				case 1:
					Log.d("x_________", "0");
				case -1:
				case -2: {
					String errorMsg = jsonRes.getString("error_msg");
					// String text = getString(R.string.pay_callback_toast,
					// errorCode, errorMsg);
					// Toast.makeText(SdkUserBaseActivity.this, text,
					// Toast.LENGTH_SHORT).show();
					isCallbackParseOk = true;
				}
					break;
				default:
					break;
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}

			// 用于测试数据格式是否异常。
			/*
			 * if (!isCallbackParseOk) { Toast.makeText(AppActivity.this,
			 * getString(R.string.data_format_error), Toast.LENGTH_LONG).show();
			 * }
			 */
		}
	};
	
	 @Override
	    protected void onDestroy() {
	        super.onDestroy();
	        Matrix.destroy(this);
	        if (mTokenTask != null) {
	            mTokenTask.doCancel();
	        }

	        if (mUserInfoTask != null) {
	            mUserInfoTask.doCancel();
	        }
	    }
}

