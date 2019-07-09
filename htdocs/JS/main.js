var Token;
var Endpoint = "http://localhost/cgi-bin/Test.exe";
function getCookie(name) {
  var value = "; " + document.cookie;
  var parts = value.split("; " + name + "=");
  if (parts.length == 2)
    return parts
      .pop()
      .split(";")
      .shift();
}
function CallServer(parameter) {
  var Prom = jQuery.Deferred();
  $.ajax({
    url: Endpoint,
    method: "Post",
    data: parameter
  })
    .done(function(data) {
      console.log("ERFOLG");
      console.log(data);
      Prom.resolve(data);
    })
    .fail(function(data) {
      console.log("Misserfolg");
      console.log(data);
      Prom.reject();
    });
  return Prom.promise();
}
function RemoveDate() {}
// function CreateDate() {
//     var parameter = {
//         "Token": Token,
//         "Desc": $("#Desc").val(),
//         "Time": $("#Time").val(),
//         "DateName": $("#DateName").val(),
//         "Befehl": "CreateDate"
//     }
//     CallServer(parameter);
// }

function GetToken() {
  console.log("Grabbe Token:");
  var token = ctoken + "=";
  var ca = document.cookie.split(";");
  for (var i = 0; i < ca.length; i++) {
    var c = ca[i];
    while (c.charAt(0) == " ") {
      c = c.substring(1);
    }
    if (c.indexOf(name) == 0) {
      return c.substring(token.length, c.length);
    }
  }
  console.log(token);
  return token;
}

function GetDates() {
  console.log("Token benutzt:");
  console.log(
    "Ld_QoNAbkMHIPHunlRkfEKXhCYG-_hcEPsyECVaPjuuyhqmSqibczThsoweqtLcccfW_lAfKDlAjuhjxGdaYtknxMLUkGDNLEcxYrOYQYPuggjCRvxMaNkqntCpcBbbBPcOvypUidHImOZciMCKCruJXruEfOBGJikOyTQiFxZDjynK_iCokIcCvKCYxooppyRkiQzXx"
  );
  var parameter = {
    Token:
      "Ld_QoNAbkMHIPHunlRkfEKXhCYG-_hcEPsyECVaPjuuyhqmSqibczThsoweqtLcccfW_lAfKDlAjuhjxGdaYtknxMLUkGDNLEcxYrOYQYPuggjCRvxMaNkqntCpcBbbBPcOvypUidHImOZciMCKCruJXruEfOBGJikOyTQiFxZDjynK_iCokIcCvKCYxooppyRkiQzXx",
    Befehl: "GetDates"
  };
  CallServer(parameter);
}
function CreateUser() {
  var parameter = {
    Username: $("#UsernameEnter").val(),
    Password: $("#PasswordEnter").val(),
    Befehl: "CreateUser"
  };
  $.when(CallServer(parameter)).done(function(data) {
    Token = data.Server;
    createDashboard();
  });
}
function RemoveDate() {
  var parameter = {
    Befehl: "DeleteDate",
    ID:
      "NsnPUdnoSyhTWGaU-MVX_PS_OAolUQvlADfeLoUYSTnunUtGIDkKReGJGRF_PhCPbwmkvylKDSmxMiLvSheCLqdpXpDAocLwHcyoODFSHpFdRClMsla-HAQmrYGJwWShwLcs-HvOewRvKibUpmjCdeVPyXPwFkEWwpMiLeU-bMtRWTKxzzWREopZRLJtJISDtDLFQIK"
  };
  $.when(CallServer(parameter))
    .done(function(data) {
      console.log(data);
    })
    .fail(function(data) {
      console.log(data);
    });
}
function ModifyDate() {
  var parameter = {
    Befehl: "ModifyDate",
    ID:
      "KqnaTsF-UqTdNbSFIsbqfgPEjBzWShQFBqDFHVfPmrRTfUgiFxkqkvlpErPSxKqfRA-yrYaCcD_vLjHfWMNqGBbBX_dYTJtfArvBQZDszQYNGMtPEAfKlzVZIgFujteYqwxmrLHPHNXG-oEryu_sTVaJtBlnW_qOLewdFYmnJUWodtOVhzBqllLnTBHjPYAxgOAeTybS",
    Time: "Letzte Woche Samstag",
    DateName: "Ich wurde verändert",
    Desc:
      "Ich liebe Gruppenarbeiten. Ich mein, ich hatte mal sowas wie Motivation, aber hey. Solange man sich die Gute Note abstauben kann nicht wahr?"
  };
  $.when(CallServer(parameter))
    .done(function(data) {
      console.log(data);
    })
    .fail(function(data) {
      console.log(data);
    });
}
function CreateDate() {
  var parameter = {
    Befehl: "CreateDate",
    Token:
      "Ld_QoNAbkMHIPHunlRkfEKXhCYG-_hcEPsyECVaPjuuyhqmSqibczThsoweqtLcccfW_lAfKDlAjuhjxGdaYtknxMLUkGDNLEcxYrOYQYPuggjCRvxMaNkqntCpcBbbBPcOvypUidHImOZciMCKCruJXruEfOBGJikOyTQiFxZDjynK_iCokIcCvKCYxooppyRkiQzXx",
    Time: "Naechste Woche Dienstag",
    DateName: "DerLustigeMannAusDemInternet",
    Desc: "Hier ist der Lustig Mann aus dem Internet"
  };
  $.when(CallServer(parameter))
    .done(function(data) {
      console.log(data);
    })
    .fail(function(data) {
      console.log(data);
    });
}
function ServermakeToken() {
  var parameter = {
    Username: $("#UsernameEnter").val(),
    Password: $("#PasswordEnter").val(),
    Befehl: "GetToken"
  };
  $.when(CallServer(parameter)).done(function(data) {
    Token = GetToken();
    createDashboard();
  });
}
async function createDashboard() {
  $.when(GetDates()).done(function() {
    window.location("#popup");
  });
}
$(document).ready(function() {
  //WIP
});
