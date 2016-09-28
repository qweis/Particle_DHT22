function collectData() {
  var  sheet = SpreadsheetApp.getActiveSheet();

  var response = UrlFetchApp.fetch("https://api.spark.io/v1/devices/53ff71065075535158281687/result?access_token=49ec93d471e99cc2255af9947dac78e8af2276c4");

  try {
    var response = JSON.parse(response.getContentText()); // parse the JSON the Particle API created
    var result = unescape(response.result); // you'll need to unescape before your parse as JSON

    try {
      var p = JSON.parse(result); // parse the JSON you created
      var d = new Date(); // time stamps are always good when taking readings
      sheet.appendRow([d, p.tc, p.tf, p.h]); // append the date and data to the sheet
    } catch(e)
    {
      Logger.log("Unable to do second parse");
    }
  } catch(e)
  {
    Logger.log("Unable to returned JSON");
  }
}