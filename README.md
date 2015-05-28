# TS Content

Importing default content using migrate module and google spreadsheets.

To make the Google doc available to the migrate module:
```File->Publish to Web->Entire Document```

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
