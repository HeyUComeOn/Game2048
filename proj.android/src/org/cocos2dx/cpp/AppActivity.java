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
		// �ڸ���������ʵ����AppDelegate
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
				 * if(msg.arg1==1) { //��½ doSdkLogin(false,true);
				 * 
				 * } else if(msg.arg2==2) { //���� doSdkPay(false, false, true); }
				 */
				switch (msg.what) {

				case 1:
					// �ȵ�½
					doSdkLogin(false, true);
					break;

				case 2:

					// �ٸ���
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
	 * case 1: //��½ doSdkPay(false, false, true); break;
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
	 * * ���ɵ���360SDK��¼�ӿڵ�Intent * * @param isLandScape �Ƿ���� * @param
	 * isBgTransparent �Ƿ񱳾�͸�� * @return Intent
	 */
	// private Intent getLoginIntent(boolean isLandScape, boolean
	// isBgTransparent) {
	// return getLoginIntent(isLandScape, isBgTransparent);
	// }

	/***
	 * ���ɵ���360SDK��¼�ӿڵ�Intent
	 * 
	 * @param isLandScape
	 *            �Ƿ����
	 * @param isBgTransparent
	 *            �Ƿ񱳾�͸��
	 * @param appKey
	 *            Ӧ�û���Ϸ��AppKey
	 * @param appChannel
	 *            Ӧ�û���Ϸ���Զ���������
	 * @return Intent
	 */
	private Intent getLoginIntent(boolean isLandScape, boolean isBgTransparent) {
		Bundle bundle = new Bundle(); // ������ز�����360SDK�����Ƿ��Ժ�����ʾ��
		bundle.putBoolean(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE,
				isLandScape);
		// ������ز�����360SDK��¼���汳���Ƿ�͸����
		bundle.putBoolean(ProtocolKeys.IS_LOGIN_BG_TRANSPARENT, isBgTransparent);
		// *** ���·ǽ�����ز��� ***
		// ���������ʹ��360SDK�ĵ�¼ģ�顣
		bundle.putInt(ProtocolKeys.FUNCTION_CODE,
				ProtocolConfigs.FUNC_CODE_LOGIN);
		Intent intent = new Intent(this, ContainerActivity.class);
		intent.putExtras(bundle);
		return intent;
	}

	// ��¼��ע��Ļص�
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
	 * ��Json�ַ��л�ȡ��Ȩ��
	 * 
	 * @param data
	 *            Json�ַ���
	 * @return ��Ȩ��
	 */
	private String parseAuthorizationCode(String data) {
		String authorizationCode = null;
		if (!TextUtils.isEmpty(data)) {
			boolean isCallbackParseOk = false;
			try {
				JSONObject json = new JSONObject(data);
				int errCode = json.getInt("errno");
				if (errCode == 0) {
					// ֻ֧��code��½ģʽ
					JSONObject content = json.getJSONObject("data");
					if (content != null) {
						// 360SDK��¼���ص�Authorization Code����Ȩ�룬60����Ч����
						authorizationCode = content.getString("code");
						isCallbackParseOk = true;
					}
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}

			// // ���ڲ������ݸ�ʽ�Ƿ��쳣��
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
            /*// ��ʾ�û�����
            mProgress = ProgressUtil.show(this, R.string.get_token_title,
                    R.string.get_token_message, new OnCancelListener() {

                        @Override
                        public void onCancel(DialogInterface dialog) {
                            if (mTokenTask != null) {
                                mTokenTask.doCancel();
                            }
                        }
                    });*/

            // ����Ӧ�÷���������AuthorizationCode��ȡAccessToken
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
	            // ����QihooUserInfo
	            mQihooUserInfo = userInfo;

	            // ������ʾQihooUser��Id��Name
	            //updateUserInfoUi();
	            Intent intent = getIntent();
	            intent.putExtra(Constants.TOKEN_INFO, mTokenInfo.toJsonString());
	            intent.putExtra(Constants.QIHOO_USER_INFO, mQihooUserInfo.toJsonString());

	        } else {
	            Toast.makeText(this, "get userinfo failed", Toast.LENGTH_LONG).show();
	        }
///////////
		if (userInfo != null && userInfo.isValid()) {
			Toast.makeText(this, "��¼�ɹ�", Toast.LENGTH_LONG).show();
			startGameScene();
		} else {
			Toast.makeText(this, "��¼ʧ��", Toast.LENGTH_LONG).show();
		}

	}

	@Override
	public void onGotTokenInfo(TokenInfo tokenInfo) {

		 if (tokenInfo == null || TextUtils.isEmpty(tokenInfo.getAccessToken())) {
	            //ProgressUtil.dismiss(mProgress);
	            Toast.makeText(this, "δ��Ӧ�÷�������ȡAccess Token", Toast.LENGTH_LONG).show();
	        } else {
	            // ����TokenInfo
	            mTokenInfo = tokenInfo;
	            mUserInfoTask = QihooUserInfoTask.newInstance();
	            // ������ʾAccessToken
	            //updateUserInfoUi();

	            /*// ��ʾ�û�����
	            ProgressUtil.setText(mProgress, getString(R.string.get_user_title),
	                    getString(R.string.get_user_message), new OnCancelListener() {

	                        @Override
	                        public void onCancel(DialogInterface dialog) {
	                            if (mUserInfoTask != null) {
	                                mUserInfoTask.doCancel();
	                            }
	                        }
	                    });*/

	            // ����Ӧ�÷���������AccessToken��ȡUserInfo
	            mUserInfoTask.doRequest(this, tokenInfo.getAccessToken(), Matrix.getAppKey(this),
	                    this);
	        }
	}

	/**
	 * ʹ��360SDK��֧���ӿ�
	 * 
	 * @param isLandScape
	 *            �Ƿ������ʾ֧������
	 * @param isFixed
	 *            �Ƿ񶨶�֧��
	 */
	protected void doSdkPay(final boolean isLandScape, final boolean isFixed,
			final boolean isBgTransparent) {

		// ֧����������
		Intent intent = getPayIntent(isLandScape, isFixed);

		// ���������ʹ��360SDK��֧��ģ�顣
		intent.putExtra(ProtocolKeys.FUNCTION_CODE,
				ProtocolConfigs.FUNC_CODE_PAY);

		// ������ز�����360SDK��¼���汳���Ƿ�͸����
		intent.putExtra(ProtocolKeys.IS_LOGIN_BG_TRANSPARENT, isBgTransparent);

		Matrix.invokeActivity(this, intent, mPayCallback);
	}

	/**
	 * ���ӷ���������ʹ��֧��������ʵ��getQihooPayInfo
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
     * @param moneyAmount �������ʹ���߿��������趨��������Ϊ100����������360SDK���ж���֧�����̣�
     *            �����Ϊ0��360SDK���в�����֧�����̡�
     * @return QihooPay
     */
    private QihooPayInfo getQihooPay(String moneyAmount) {
        // ����QihooPay
        QihooPayInfo qihooPay = new QihooPayInfo();

        // ��¼�õ�AccessToken��UserId������֧����
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

        // ��ѡ����
        qihooPay.setAppExt1(getString(R.string.demo_pay_app_ext1));
        qihooPay.setAppExt2(getString(R.string.demo_pay_app_ext2));
        qihooPay.setAppOrderId("");

        return qihooPay;
    }

	/***
	 * ���ɵ���360SDK֧���ӿڻ���������Intent
	 * 
	 * @param isLandScape
	 * @param pay
	 * @return Intent
	 */
	protected Intent getPayIntent(boolean isLandScape, boolean isFixed) {

		Bundle bundle = new Bundle();

		QihooPayInfo pay = getQihooPayInfo(isFixed);

		// ������ز�����360SDK�����Ƿ��Ժ�����ʾ��
		bundle.putBoolean(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE,
				isLandScape);

		// *** ���·ǽ�����ز��� ***

		// ����QihooPay�еĲ�����
		// ����������û�access token��Ҫʹ��ע����ں�ˢ�����⣬���64�ַ���
		bundle.putString(ProtocolKeys.ACCESS_TOKEN, pay.getAccessToken());

		// ���������360�˺�id��������
		bundle.putString(ProtocolKeys.QIHOO_USER_ID, pay.getQihooUserId());

		// �����������������Ʒ���, �Է�Ϊ��λ�������ڵ���100�֣�360SDK���ж���֧�����̣� �����Ϊ0��360SDK���в�����֧�����̡�
		bundle.putString(ProtocolKeys.AMOUNT, pay.getMoneyAmount());

		// ������������������Ϸ��ֵ�ҵ�Ĭ�ϱ���������2������1Ԫ����ҿ��Զһ�2����Ϸ�ң�������
		bundle.putString(ProtocolKeys.RATE, pay.getExchangeRate());

		// �����������������Ʒ���ƣ�Ӧ��ָ�����������ģ����10�������֡�
		bundle.putString(ProtocolKeys.PRODUCT_NAME, pay.getProductName());

		// ���������������Ʒ����Ʒid��Ӧ��ָ�������16�ַ���
		bundle.putString(ProtocolKeys.PRODUCT_ID, pay.getProductId());

		// ���������Ӧ�÷��ṩ��֧�����֪ͨuri�����255�ַ���360����������֧���ӿڻص�����uri������Э����鿴�ĵ��У�֧�����֪ͨ�ӿڨCӦ�÷������ṩ�ӿڡ�
		bundle.putString(ProtocolKeys.NOTIFY_URI, pay.getNotifyUri());

		// �����������Ϸ��Ӧ�����ƣ����16�����֡�
		bundle.putString(ProtocolKeys.APP_NAME, pay.getAppName());

		// ���������Ӧ���ڵ��û���������Ϸ��ɫ���� ��Ӧ���ڰ�360�˺ź�Ӧ���˺ţ������360�û��������16�����֡�����ֵ����������
		// �䵽ͳһ���û��˻�����������ɫ����ʹ�ã���
		bundle.putString(ProtocolKeys.APP_USER_NAME, pay.getAppUserName());

		// ���������Ӧ���ڵ��û�id��
		// ��Ӧ���ڰ�360�˺ź�Ӧ���˺ţ���ֵ�����������䵽ͳһ���û��˻�����������ɫ����ʹ�ã������360�û�ID���32�ַ���
		bundle.putString(ProtocolKeys.APP_USER_ID, pay.getAppUserId());

		// ��ѡ������Ӧ����չ��Ϣ1��ԭ�����أ����255�ַ���
		bundle.putString(ProtocolKeys.APP_EXT_1, pay.getAppExt1());

		// ��ѡ������Ӧ����չ��Ϣ2��ԭ�����أ����255�ַ���
		bundle.putString(ProtocolKeys.APP_EXT_2, pay.getAppExt2());

		// ��ѡ������Ӧ�ö����ţ�Ӧ���ڱ���Ψһ�����32�ַ���
		bundle.putString(ProtocolKeys.APP_ORDER_ID, pay.getAppOrderId());

		Intent intent = new Intent(this, ContainerActivity.class);
		intent.putExtras(bundle);

		return intent;
	}

	// ֧���Ļص�
	protected IDispatcherCallback mPayCallback = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
			Log.d(TAG, "mPayCallback, data is " + data);
			boolean isCallbackParseOk = false;
			JSONObject jsonRes;
			try {
				jsonRes = new JSONObject(data);
				// error_code ״̬�룺 0 ֧���ɹ��� -1 ֧��ȡ���� 1 ֧��ʧ�ܣ� -2 ֧�������С�
				// error_msg ״̬����
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

			// ���ڲ������ݸ�ʽ�Ƿ��쳣��
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
