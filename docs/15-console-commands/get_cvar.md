# get_cvar

Get the value of a console variable.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| name | string | Yes | Name of the console variable |

## Returns

On success, returns the console variable name and its current value in the format `name = value`.

On error, returns an error message (e.g. if the console variable does not exist).

## Example

```json
{
  "name": "r.ScreenPercentage"
}
```

## Notes

- Console variables (CVars) are used to configure various engine and game settings at runtime.
- The returned value is always a string representation, regardless of the CVar's underlying type (int, float, string).
- Common CVar prefixes include `r.` (rendering), `p.` (physics), `t.` (threading), and `a.` (audio).
