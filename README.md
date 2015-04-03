# TS Content

Importing default content using migrate module and google spreadsheets.

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
