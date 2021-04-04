// This file contains app-specific configuration options for use with dw-auth.js

$ = jQuery;

var geo = $('meta[name="geo.country"]').attr('content');
// Convert geo to locale and select associated sign-in and register text
if (geo == 'ZZ') {
    locale = "en_US";
    dwSignin = "Sign in";
    dwRegister = "Register";
    dwProfileSignOut = "Sign out";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/";
    dwProfileSettings = "Settings";
    dwProfileLang = "en";
    dwProfileCommunities = "Communities";
    dwProfileMain = "My developerWorks";
    dwProfileMainURL = "https://developer.ibm.com/mydeveloperworks";    
    dwProfileMyIBM = "My IBM";    
} else if (geo == 'BR') {
    locale = "pt_BR";
    dwSignin = "Faça o login";
    dwRegister = "Registre-se";
    dwProfileSignOut = "Sair";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/br/";
    dwProfileSettings = "Configurações";
    dwProfileLang = "pt_br";
    dwProfileCommunities = "Comunidades";
    dwProfileMain = "Meu perfil";
    dwProfileMainURL = "/developerworks/community/profiles/html/myProfileView.do?lang=" + dwProfileLang;
    dwProfileMyIBM = "Minha IBM";
} else if (geo == 'CN') {
    locale = "zh_CN";
    dwSignin = "登录";
    dwRegister = "注册";
    dwProfileSignOut = "退出";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/cn/";
    dwProfileSettings = "设置";
    dwProfileLang = "zh";
    dwProfileCommunities = "社区";
    dwProfileMain = "我的概要信息";
    dwProfileMainURL = "/developerworks/community/profiles/html/myProfileView.do?lang=" + dwProfileLang;
    dwProfileMyIBM = "我的 IBM";    
} else if (geo == 'JP') {
    locale = "ja_JP";
    dwSignin = "サインイン";
    dwRegister = "登録";
    dwProfileSignOut = "サイン・アウト";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/jp/";
    dwProfileSettings = "設定";
    dwProfileLang = "ja";
    dwProfileCommunities = "コミュニティー";
    dwProfileMain = "マイ・プロフィール";
    dwProfileMainURL = "/developerworks/community/profiles/html/myProfileView.do?lang=" + dwProfileLang;
    dwProfileMyIBM = "My IBM";    
} else if (geo == 'RU') {
    locale = "ru_RU";
    dwSignin = "Войти";
    dwRegister = "Зарегистрироваться";
    dwProfileSignOut = "Выйти";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/ru/";
    dwProfileSettings = "Настройки";
    dwProfileLang = "ru";
    dwProfileCommunities = "Сообщества";
    dwProfileMain = "Мой профайл";
    dwProfileMainURL = "/developerworks/community/profiles/html/myProfileView.do?lang=" + dwProfileLang;
    dwProfileMyIBM = "Мой IBM";    
} else if (geo == 'AR,CO,PE,MX,BO,CL,EC,PY,UY,VE') {
    locale = "es_AR";
    dwSignin = "Inscríbase";
    dwRegister = "Regístrese";
    dwProfileSignOut = "Cerrar Sesión";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/ssa/";
    dwProfileSettings = "Configuración";
    dwProfileLang = "es";
    dwProfileCommunities = "Comunidades";
    dwProfileMain = "Mi perfil";
    dwProfileMainURL = "/developerworks/community/profiles/html/myProfileView.do?lang=" + dwProfileLang;
    dwProfileMyIBM = "Mi IBM";    
} else if (geo == 'VN') {
    locale = "en_US";
    dwSignin = "Sign in";
    dwRegister = "Register";
    dwProfileSignOut = "Sign out";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/";
    dwProfileSettings = "Settings";
    dwProfileLang = "en";
    dwProfileCommunities = "Communities";
    dwProfileMain = "My profile";
    dwProfileMainURL = "/developerworks/community/profiles/html/myProfileView.do?lang=" + dwProfileLang;
    dwProfileMyIBM = "My IBM";    
} else {
    locale = "en_US";
    dwSignin = "Sign in";
    dwRegister = "Register";
    dwProfileSignOut = "Sign out";
    dwProfileSignOutURL = "https://www.ibm.com/developerworks/";
    dwProfileSettings = "Settings";
    dwProfileLang = "en";
    dwProfileCommunities = "Communities";
    dwProfileMain = "My developerWorks";
    dwProfileMainURL = "https://developer.ibm.com/mydeveloperworks";
    dwProfileMyIBM = "My IBM";    
} 

var dw_auth_prop = {

	signoutUrl: dwProfileSignOutURL,
	appCode: 'dwmav'

}

console.log("geo is: " + geo);
console.log("locale is: " + locale);
