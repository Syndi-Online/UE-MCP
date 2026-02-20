# get_config_value

Read a value from project configuration files.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| config | string | Yes | Config file name: Engine, Game, Editor, Input, Scalability |
| section | string | Yes | Config section name (e.g. /Script/Engine.RendererSettings) |
| key | string | Yes | Config key name |

## Returns

On success, returns the value in the format `[section] key = value`.

On error, returns an error message describing the failure (e.g. section or key not found).

## Example

```json
{
  "config": "Engine",
  "section": "/Script/Engine.RendererSettings",
  "key": "r.DefaultFeature.AntiAliasing"
}
```

## Response

### Success
```
[/Script/Engine.RendererSettings] r.DefaultFeature.AntiAliasing = 2
```

### Error
```
Missing required parameter: config
```
```
Missing required parameter: section
```
```
Missing required parameter: key
```
```
Failed to get config value: Key not found in section
```

## Notes

- Valid config file names are: `Engine`, `Game`, `Editor`, `Input`, `Scalability`.
- Section names typically follow the pattern `/Script/ModuleName.ClassName`.
- This reads from the project's config files (e.g. DefaultEngine.ini, DefaultGame.ini).
- Use `set_config_value` to modify configuration values.
