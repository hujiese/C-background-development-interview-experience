
  $ = jQuery; // needed for nav pages

  IBMCore.common.util.config.set({
      masthead: { 
         type: "alternate",
         greeting: { 
             enabled: false 
         }
      },
    footer: { type: "alternate" }, 
    survey: { global_percent: 0 }
    });

  var userLinks = [];
  var secure = "https://";
  var domain = window.location.hostname;
  var signedIn = false;
  var notificationsCount = null;
  var curUrl = window.location.href;
  curUrl.replace("http:", "https:");
  var dest = encodeURIComponent(curUrl);
  var signoutUrl = new String('');
  var apps = ["dwmav", "dwrfe", "dwcomm", "dwsecapp", "dwapi"]
  var app = new String('');
  var validApp = false;
  var screenname = "";
  if (typeof(dw_auth_prop) == 'undefined') {
      console.log("dw_auth_prop is undefined")
  } else {
      app = dw_auth_prop.appCode;
      if (app != '') {
        for (i = 0; i < apps.length; i++) {
          if (app === apps[i]) { 
            validApp = true;
            break; 
          } // match
        }
      }
      signoutUrl = dw_auth_prop.signoutUrl;
  }
  // If not known app, default to dwmav
  if (!validApp) {
      app = "dwmav";
  }
  // If dest not set, default to home page
  if (signoutUrl == '') {
      signoutUrl = domain + '/developerworks/'
    } 
  
  // Profile Menu options
  var profileMenuMyIBM = "    <li class=\"ibm-dropdown-menu-primary\" role=\"presentation\" data-linktype=\"notifications\">" +
    "       <a class=\"\" href=\"https://myibm.ibm.com/dashboard/?lnk=mmi\" tabindex=\"0\">" + dwProfileMyIBM + "</a>" +
    "     </li>";
  var profileMenudWHeading = "    <li class=\"ibm-dropdown-menu-primary menu-heading\" role=\"presentation\">" +
    "       <span tabindex=\"0\">IBM Developer</span>" +
    "     </li>";
  var profileMenudW = "    <li class=\"ibm-dropdown-menu-primary\" role=\"presentation\">" +
    "       <a href=\"https://www.ibm.com/developerworks/community/groups/service/html/mycommunities?lang=" + dwProfileLang + "\" tabindex=\"-1\">" +
    "         <span>" + dwProfileCommunities + "</span>" +
    "       </a>" +
    "     </li>" +
    "     <li class=\"ibm-dropdown-menu-primary\" role=\"presentation\">" +
    "       <a href=\"https://www.ibm.com/developerworks/community/news/?lang=" + dwProfileLang + "\" tabindex=\"-1\">" +
    "         <span>" + dwProfileSettings + "</span>" +
    "       </a>" +
    "     </li>";
  var profileMenuSignIn = "   <li class=\"ibm-dropdown-menu-primary ibm-signin-alt\" role=\"presentation\">" +
        "     <a href=\"/developerworks/dwwi/jsp/ssologin.jsp?d=" + dest + "&lang=" + dwProfileLang + "&a=" + app + "\" role=\"menuitem\">" + dwSignin + "</a>" +
        "   </li>" +
        "   <li class=\"ibm-dropdown-menu-primary dw-register-alt\" role=\"presentation\">" +
        "     <a href=\"/developerworks/dwwi/jsp/ssoregister.jsp?d=" + dest + "&lang=" + dwProfileLang + "&a=" + app + "\" role=\"menuitem\">" + dwRegister + "</a>" +
        "   </li>";
  var profileMenuSignOut = "    <li class=\"ibm-dropdown-menu-primary signinout\" role=\"presentation\">" +
    "     <a href=\"/developerworks/dwwi/jsp/ssologout.jsp?d=" + signoutUrl + "&a=" + app + "\" role=\"menuitem\">" + dwProfileSignOut + "</a>" +
    "   </li>";

  
    // If viewport below 480, do not show screenname
    var showScreenName = false; // default
/*    var showScreenName = true; // default
    if ($(window).width() < 480) {
    showScreenName = false; // do not show
  }
*/

  // Wait until the masthead main onscreen items have been generated
  IBMCore.common.module.masthead.subscribe("ready", "customjs", function() {
      // Uncomment if you want to show the link contents (text) you changed to above, instead of the default v18 "you're signed in" link/image.
      //IBMCore.common.module.masthead.showProfileLinkContents();
    });

  // Wait until the profile menu has been generated.
  IBMCore.common.module.masthead.subscribe("profileMenuReady", "customjs", function() {
    // Determine if the user is authenticated
    $.ajax({
      cache: false,
      url: secure + domain + '/developerworks/dwwi/jsp/Auth.jsp?format=json',
      dataType: 'json',
      success: function (data) {
        console.log("ajax success");
        if ((data.status == "true") && (!data.url)) {
          signedIn = true;
          // Profile menu options
          userLinks = [
          // {
          //  title: dwProfileMain,
          //  url: dwProfileMainURL
          // },
          {
            title: dwProfileCommunities,
            url: "/developerworks/community/groups/service/html/mycommunities?lang=" + dwProfileLang
          },
          {
            title: dwProfileSettings,
            url: "/developerworks/community/news/?lang=" + dwProfileLang
          },
          {
            title: dwProfileSignOut,
            url: "/developerworks/dwwi/jsp/ssologout.jsp?d=" + signoutUrl + "&a=" + app
          }];
          
          if (showScreenName) {
            IBMCore.common.module.masthead.showProfileLinkSignedin({
              linkContents: data.screenname,
              linkAlt: "Signed in as " + data.screenname
            });
            IBMCore.common.module.masthead.showProfileLinkContents();
          } else {
                        IBMCore.common.module.masthead.showProfileLinkSignedin({
              linkContents: "",
              linkAlt: "Signed in as " + data.screenname, 
              backgroundImage: "",
              type: "default"
                });
          }
          
          // $(":button.ibm-profile-link").attr("title", "Signed in as " + data.screenname);
        
          //Set country, language in IBM IPC Cookie
          IBMCore.common.util.setIpcCookie(geo, dwProfileLang);
        
          // pass ibmuniqueid to piwik
          if (typeof _paq !== "undefined") { _paq.push(['setUserId', data.ibmuniqueid]); }
                  
        } else if ((data.status == "true") && (data.url)) {
          window.location = secure + data.url + encodeURI(curUrl);
          
        } else if (data.status == "false") {
          userLinks = [{
            title: dwSignin,
            url: "/developerworks/dwwi/jsp/ssologin.jsp?d=" + dest + "&lang=" + locale + "&a=" + app
          },
          {
            title: dwRegister,
            url: "/developerworks/dwwi/jsp/ssoregister.jsp?d=" + dest + "&lang=" + locale + "&a=" + app
          }];
          
        }
        
        if (signedIn == false) {                    
            $("button.deploy").each(function(index) {
               if ($(this).prev("button.run").length > 0) {
                   $(this).attr("disabled", true);
                     $(this).prev().attr("disabled", true);
                   pefEditors[$(this).closest("div.pef").attr("index")].setOption("readOnly", true);
               }
            });
               $(".dw-intro-signin").each(function(index) {
                        $(this).css("display","inline");
            var anchor = "";
                        $(this).children("a").eq(0).attr("href","/developerworks/dwwi/jsp/ssologin.jsp?d=" + dest + anchor + "&lang=" + dwProfileLang + "&a=" + app); 
               });    
               $(".dw-inline-signin").each(function(index) {
                        $(this).css("display","inline-block");
                        var anchor = "%23" + $(this).prevAll("[id]").eq(0).attr("id");
                        if (anchor == "#undefined") anchor = ""
                        $(this).children("a").eq(0).attr("href","/developerworks/dwwi/jsp/ssologin.jsp?d=" + dest + anchor + "&lang=" + dwProfileLang + "&a=" + app); 
               });  
               $(".dw-quiz-signin").each(function(index) {
                        $(this).css("display","inline-block");
            var anchor = "%23artquiz";
                        $(this).children("a").eq(0).attr("href","/developerworks/dwwi/jsp/ssologin.jsp?d=" + dest + anchor + "&lang=" + dwProfileLang + "&a=" + app); 
               });    
        }
        changeMenu();

      },
      error: function (data) {
        console.log("ajax error");
      }
    }); //ajax

  });
  
  // Modify the profile menu.
  function changeMenu() {
    IBMCore.common.module.masthead.editProfileMenu({
      action: "replace",
      links: userLinks
    });
    
    // Replace menu from API; hack for now until implemented in API
    if (signedIn) {
      document.getElementById("ibm-signin-minimenu-container").innerHTML = profileMenuMyIBM + 
        profileMenudWHeading + profileMenudW + profileMenuSignOut;
    } else {
      document.getElementById("ibm-signin-minimenu-container").innerHTML = profileMenuMyIBM + 
        profileMenudWHeading + profileMenuSignIn;
    }

  }
  