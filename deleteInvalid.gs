function deleteInvalid() {
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var s = ss.getSheetByName('Sheet1');
  var values = s.getDataRange().getValues();

  var deleted = 0;                                        // Counter (don't need if we loop backwards)
  for (var row = 0; row < values.length; row++) {
    if (values[row][2] == 32) {                           // 0,1,2,3 = A,B,C,D scan column C for invalid data of 32
      Logger.log(row + 1 - deleted)                       // 
      s.deleteRow(row + 1 - deleted);                     // Delete row with invalid Data
      deleted++;
    }
  }
  SpreadsheetApp.flush();
};