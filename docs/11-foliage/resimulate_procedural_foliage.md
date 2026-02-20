# resimulate_procedural_foliage

Resimulate procedural foliage volumes in the current level.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| volume_name | string | No | Name of specific procedural foliage volume (empty for all volumes) |

## Returns

On success, returns a message indicating how many procedural foliage volumes were resimulated.

On error, returns an error message describing the failure.

## Example

```json
{
  "volume_name": "ProceduralFoliageVolume_0"
}
```

## Response

### Success
```
Resimulated 3 procedural foliage volume(s)
```

### Error
```
Failed to resimulate procedural foliage: No procedural foliage volumes found
```

## Notes

- If `volume_name` is omitted or empty, all procedural foliage volumes in the current level are resimulated.
- Procedural foliage volumes must already exist in the level for this tool to have any effect.
- Resimulation regenerates the foliage placement based on the volume's procedural foliage spawner settings.
