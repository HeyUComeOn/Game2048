/*package com.mitsui.game;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnCancelListener;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.widget.Toast;

import com.qihoo.gamecenter.sdk.common.IDispatcherCallback;
import com.qihoo.gamecenter.sdk.protocols.pay.ProtocolKeys;
import com.qihoopay.insdk.matrix.Matrix;
import com.test.sdk.Constants;
//import com.test.sdk.activity.FlowTestLoginActivity;
import com.test.sdk.activity.SdkUserBaseActivity;
import com.test.sdk.appserver.QihooUserInfo;
import com.test.sdk.appserver.QihooUserInfoListener;
import com.test.sdk.appserver.QihooUserInfoTask;
import com.test.sdk.appserver.TokenInfo;
import com.test.sdk.appserver.TokenInfoListener;
import com.test.sdk.appserver.TokenInfoTask;
import com.test.sdk.utils.ProgressUtil;


public class StartActivity extends SdkUserBaseActivity implements TokenInfoListener, QihooUserInfoListener, OnClickListener{

	
	
	private static final String TAG = "StartActivity";

    private TextView mLoginResultView;

    private ProgressDialog mProgress;

    private TokenInfo mTokenInfo;

    private QihooUserInfo mQihooUserInfo;

    private boolean mIsLandscape;

    private TokenInfoTask mTokenTask;
    private QihooUserInfoTask mUserInfoTask;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	
    	super.onCreate(savedInstanceState);
    	
    	Intent intent = getIntent();
        mIsLandscape = intent.getBooleanExtra(Constants.IS_LANDSCAPE, true);
        setRequestedOrientation(mIsLandscape ? ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE
                : ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        mTokenInfo = TokenInfo.parseJson(intent.getStringExtra(Constants.TOKEN_INFO));
        mQihooUserInfo = QihooUserInfo.parseJson(intent.getStringExtra(Constants.QIHOO_USER_INFO));

        //setContentView(R.layout.sdk_show_all_activity);

        if (savedInstanceState == null) {
            // 初始化
            Matrix.init(this, false, new IDispatcherCallback() {
                @Override
                public void onFinished(String data) {
                    Log.d(TAG, "matrix startup callback,result is " + data);
                }
            });
        }
    	
    }
    
	@Override
	public void onGotAuthorizationCode(String code) {
		
		if (TextUtils.isEmpty(code)) {
            Toast.makeText(this, R.string.get_code_fail, Toast.LENGTH_LONG).show();
        } else {
            clearLoginResult();
            mTokenTask = TokenInfoTask.newInstance();
            // 提示用户进度
            mProgress = ProgressUtil.show(this, R.string.get_token_title,
                    R.string.get_token_message, new OnCancelListener() {

                        @Override
                        public void onCancel(DialogInterface dialog) {
                            if (mTokenTask != null) {
                                mTokenTask.doCancel();
                            }
                        }
                    });

            // 请求应用服务器，用AuthorizationCode换取AccessToken
            mTokenTask.doRequest(this, code, Matrix.getAppKey(this), this);
        }
	}

	@Override
	public void onGotError(int errCode) {
		
		clearLoginResult();
	}

	@Override
	public void onGotUserInfo(QihooUserInfo userInfo) {

		ProgressUtil.dismiss(mProgress);

        if (userInfo != null && userInfo.isValid()) {
            // 保存QihooUserInfo
            mQihooUserInfo = userInfo;

            // 界面显示QihooUser的Id和Name
           
            Intent intent = getIntent();
            intent.putExtra(Constants.TOKEN_INFO, mTokenInfo.toJsonString());
            intent.putExtra(Constants.QIHOO_USER_INFO, mQihooUserInfo.toJsonString());

        } else {
            Toast.makeText(this, R.string.get_user_fail, Toast.LENGTH_LONG).show();
        }
		
	}

	@Override
	public void onGotTokenInfo(TokenInfo tokenInfo) {

		 if (tokenInfo == null || TextUtils.isEmpty(tokenInfo.getAccessToken())) {
	            ProgressUtil.dismiss(mProgress);
	            Toast.makeText(this, R.string.get_token_fail, Toast.LENGTH_LONG).show();
	        } else {
	            // 保存TokenInfo
	            mTokenInfo = tokenInfo;
	            mUserInfoTask = QihooUserInfoTask.newInstance();
	            // 界面显示AccessToken
	            //updateUserInfoUi();

	            // 提示用户进度
	            ProgressUtil.setText(mProgress, getString(R.string.get_user_title),
	                    getString(R.string.get_user_message), new OnCancelListener() {

	                        @Override
	                        public void onCancel(DialogInterface dialog) {
	                            if (mUserInfoTask != null) {
	                                mUserInfoTask.doCancel();
	                            }
	                        }
	                    });

	            // 请求应用服务器，用AccessToken换取UserInfo
	            mUserInfoTask.doRequest(this, tokenInfo.getAccessToken(), Matrix.getAppKey(this),
	                    this);
	        }
		
	}
	
	 private void clearLoginResult() {
	        mTokenInfo = null;
	        mQihooUserInfo = null;
	        //updateUserInfoUi();
	    }

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch (arg0.getId()) {
        case R.id.btn_login_bg_transparent:
            clearLoginResult();
            // 调用360SDK登录（是否横屏显示、透明背景）
            doSdkLogin(mIsLandscape, true);
            break;

        case R.id.btn_login:
            clearLoginResult();
            // 调用360SDK登录（是否横屏显示、不透明背景）
            doSdkLogin(mIsLandscape, false);
            break;

        case R.id.btn_fixed_pay_bg_transparent:
            // 调用360SDK定额支付（是否横屏显示、透明背景）
            doSdkPay(mIsLandscape, true, true);
            break;

        case R.id.btn_fixed_pay_bg_visible:
            // 调用360SDK定额支付（是否横屏显示、不透明背景）
            doSdkPay(mIsLandscape, true, false);
            break;

        case R.id.btn_not_fixed_pay_bg_transparent:
            // 调用360SDK不定额支付（是否横屏显示、透明背景）
            doSdkPay(mIsLandscape, false, true);
            break;

        case R.id.btn_not_fixed_pay_bg_visible:
            // 调用360SDK不定额支付（是否横屏显示、不透明背景）
            doSdkPay(mIsLandscape, false, false);
            break;

        case R.id.btn_flow_test:
            // 打开流程测试登录界面
            Intent intent = new Intent(this, FlowTestLoginActivity.class);
            intent.putExtra(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE, mIsLandscape);
            startActivity(intent);
            break;

        case R.id.btn_quit:
            // 调用360SDK退出（是否横屏显示）
            doSdkQuit(mIsLandscape);
            break;

        case R.id.btn_real_name_register_bg_transparent: {
            if (mQihooUserInfo == null || mTokenInfo == null) {
                Toast.makeText(this, "请先登录", Toast.LENGTH_LONG).show();
                return;
            }
            // 实名注册（是否横屏显示）
            doSdkRealNameRegister(mIsLandscape, true, mQihooUserInfo.getId());
        }
            break;
        case R.id.btn_real_name_register_bg_visible: {
            if (mQihooUserInfo == null || mTokenInfo == null) {
                Toast.makeText(this, "请先登录", Toast.LENGTH_LONG).show();
                return;
            }
            // 实名注册（是否横屏显示）
            doSdkRealNameRegister(mIsLandscape, false, mQihooUserInfo.getId());
        }
            break;
        case R.id.btn_anti_addiction_query: {
            if (mQihooUserInfo == null || mTokenInfo == null) {
                Toast.makeText(this, "请先登录", Toast.LENGTH_LONG).show();
                return;
            }
            // 防沉迷查询
            doSdkAntiAddictionQuery(mQihooUserInfo.getId(), mTokenInfo.getAccessToken());
        }
            break;
        case R.id.btn_switch_account_bg_transparent:
            // 调用360SDK切换账号（是否横屏显示、透明背景）
            doSdkSwitchAccount(mIsLandscape, true);
            break;
        case R.id.btn_switch_account:
            // 调用360SDK切换账号（是否横屏显示、不透明背景）
            doSdkSwitchAccount(mIsLandscape, false);
            break;
        case R.id.btn_self_check:
            doSdkSelfCheck();
            break;
        case R.id.btn_get_out_sdk_info:
            doSdkProInfoQuery();
            break;
        case R.id.btn_bbs:
            doSdkBBS(mIsLandscape);
            break;
        case R.id.btn_bind: {
            if (mQihooUserInfo == null || mTokenInfo == null) {
                Toast.makeText(this, "请先登录", Toast.LENGTH_LONG).show();
                return;
            }
            doSdkBindPhoneNum(mIsLandscape);
        }
            break;
        case R.id.btn_bbs_post:
            doSdkBBSPost(mIsLandscape);
            break;
        case R.id.btn_update_test: {
            //startUpdateTest();
            break;
        }

        default:
            break;
    }
	}

}
*/