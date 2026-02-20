# set_config_value

Write a value to project configuration files.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| config | string | Yes | Config file name: Engine, Game, Editor, Input, Scalability |
| section | string | Yes | Config section name |
| key | string | Yes | Config key name |
| value | string | Yes | Value to set |

## Returns

On success, returns the written value in the format `[section] key = value`.

On error, returns an error message describing the failure.

## Example

```json
{
  "config": "Game",
  "section": "/Script/EngineSettings.GeneralProjectSettings",
  "key": "ProjectVersion",
  "value": "1.0.0"
}
```

## Notes

- Valid config file names are: `Engine`, `Game`, `Editor`, `Input`, `Scalability`.
- Changes are written to the project's config files (e.g. DefaultEngine.ini, DefaultGame.ini).
- Config changes are persistent and will survive editor restarts.
- Be careful when modifying engine configuration values, as incorrect settings may cause issues on the next editor launch.
- Use `get_config_value` to read the current value before modifying it.
