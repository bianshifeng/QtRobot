import QtQuick 2.0

QtObject{
    id: id_root

    property Loader mainPageLoader:id_page_loader
    property Loader pwdPageLoader:id_pwd_loader


    property string pageLoadName:"LoadPage"
    property string pageMainName:"MainPage"
    property string pageTalkName:"TalkPage"
    property string pageYYYName:"YYYPage"
    property string pageConfigName:"ConfigPage"
    property string pagePwdName:"PassworkPage"
    property string pageShowCaseName:"ShowCasePage"
    property string pageCruiseName:"CruisePage"


    property Component __mainPageCom: null
    property Component __talkPageCom: null
    property Component __yyyPageCom: null
    property Component __configPageCom: null
    property Component __loadPageCom: null
    property Component __pwdPageCom: null
    property Component __showCasePageCom: null
    property Component __cruisePageCom: null

    function __loadPageMain(){
        if (!__mainPageCom) {
            __mainPageCom = Qt.createComponent("qrc:/qml/PagesV5/PageMain.qml")
        }
        if (__mainPageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __mainPageCom;
            mainPageLoader.item.pageName = id_root.pageMainName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }

    }
    function __loadPageTalk(){
        if (!__talkPageCom) {
            __talkPageCom = Qt.createComponent("qrc:/qml/Pages/PageTalk.qml")
        }
        if (__talkPageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __talkPageCom;
            mainPageLoader.item.pageName = id_root.pageMainName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }
    }
    function __loadPageLoad(){
        if (!__loadPageCom) {
            __loadPageCom = Qt.createComponent("qrc:/qml/PagesV5/PageLoad.qml")
        }
        if (__loadPageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __loadPageCom;
            mainPageLoader.item.pageName = id_root.pageMainName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }
    }
    function __loadConfigPage(){
        if (!__configPageCom) {
            __configPageCom = Qt.createComponent("qrc:/qml/PagesV5/PageConfig.qml")
        }
        if (__configPageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __configPageCom;
            mainPageLoader.item.pageName = id_root.pageConfigName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }
    }
    function __loadYYYPage(){
        if (!__yyyPageCom) {
            __yyyPageCom = Qt.createComponent("qrc:/qml/Pages/PageShake.qml")
        }
        if (__yyyPageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __yyyPageCom;
            mainPageLoader.item.pageName = id_root.pageYYYName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }
    }

    function __loadShowCasePage(){
        if (!__showCasePageCom) {
            __showCasePageCom = Qt.createComponent("qrc:/qml/PagesV5/PageShowCase.qml")
        }
        if (__showCasePageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __showCasePageCom;
            mainPageLoader.item.pageName = id_root.pageShowCaseName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }
    }
    function __loadCruisePage(){
        if (!__cruisePageCom) {
            __cruisePageCom = Qt.createComponent("qrc:/qml/PagesV5/PageCruise.qml")
        }
        if (__cruisePageCom.status == Component.Ready && mainPageLoader) {
            mainPageLoader.sourceComponent = __cruisePageCom;
            mainPageLoader.item.pageName = id_root.pageCruiseName;
            mainPageLoader.item.init();
            mainPageLoader.forceActiveFocus();
        }
    }



    function __loadPwdPage(currentPageName,enterPageName){
        if (!__pwdPageCom) {
            __pwdPageCom = Qt.createComponent("qrc:/qml/PagesV5/PagePassword.qml")
        }
        if (__pwdPageCom.status == Component.Ready && pwdPageLoader) {
            pwdPageLoader.sourceComponent = __pwdPageCom;
            pwdPageLoader.item.pageName = id_root.pagePwdName;
            pwdPageLoader.item.currentPageName = currentPageName;
            pwdPageLoader.item.wantEnterPageName = enterPageName;
            pwdPageLoader.item.init();
            pwdPageLoader.forceActiveFocus();
        }
    }


    function __closePwdPage(){
        __resetPwdLoader()
    }

    function __resetPwdLoader(){
        pwdPageLoader.sourceComponent= undefined
    }

    function navigatPage(currentPageName,enterPageName,isNeedPwd){

        __resetPwdLoader();

        if(isNeedPwd){
            id_root.__loadPwdPage(currentPageName,enterPageName);
            return;
        }

        switch(enterPageName){

        case id_root.pageLoadName:
            __loadPageLoad();
            break;
        case id_root.pageMainName:
            __loadPageMain();
            break;
        case id_root.pageTalkName:
            __loadPageTalk()
            break;
        case id_root.pageYYYName:
            __loadYYYPage()
            break;
        case id_root.pageConfigName:
            __loadConfigPage()
            break;
        case id_root.pageShowCaseName:
            __loadShowCasePage()
            break;
        case id_root.pageCruiseName:
            __loadCruisePage()
            break;
        default:
            break;

        }


    }

}

