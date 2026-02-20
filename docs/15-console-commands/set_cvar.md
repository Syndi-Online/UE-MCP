# set_cvar

Set the value of a console variable.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| name | string | Yes | Name of the console variable |
| value | string | Yes | New value for the console variable |

## Returns

On success, returns a confirmation in the format `name set to value`.

On error, returns an error message (e.g. if the console variable does not exist or the value is invalid).

## Example

```json
{
  "name": "r.ScreenPercentage",
  "value": "100"
}
```

## Response

### Success
```
r.ScreenPercentage set to 100
```

### Error
```
Missing required parameter: name
```
```
Missing required parameter: value
```
```
Console variable 'r.InvalidCVar' not found
```

## Notes

- The value is passed as a string and will be parsed according to the CVar's type (int, float, string).
- Changes made via CVars are typically not persistent across editor sessions unless the CVar is saved to a config file.
- Some CVars are read-only and cannot be modified at runtime.
- Be cautious when changing engine CVars, as incorrect values may affect editor stability or performance.
