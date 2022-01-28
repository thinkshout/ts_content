# TS Content

Importing default content using migrate module and google spreadsheets.

## Initial setup

To make the Google Sheet available to the migrate module, change the sharing permissions to: *Anyone on the internet with this link can view*

Generate an API key for the Google Sheets API. Instructions: https://cloud.google.com/docs/authentication/api-keys

Test that your sheet data is accessible here:

```
https://sheets.googleapis.com/v4/spreadsheets/SPREADSHEET_ID/values/Sheet1?key=API_KEY
```

- Replace `SPREADSHEET_ID` with the Google Sheet ID copied from the URL.
- Replace `API_KEY` with your api key.
- If the sheet tab with your data is named something else, replace `Sheet1` with the name of the sheet tab.

## Run the migration

After updating migration mappings re-register the migrations with:

```
drush migrate-register
```

Run your migrations with

```
drush mi --all
```

or

```
drush mi --group=<GROUP>
```

## Upgrade to Google Sheets API v4

Review the code and upgrade instructions in `ts_content.migrations.v4.inc`
