# find_referencers_of_class

Find all assets that reference a given C++ or Blueprint class.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| class_path | string | Yes | Class path, e.g. /Script/HoldMyWheel.RepairActivity |
| package_path | string | No | Limit search to this package path, e.g. /Game/BluePrint |
| recursive | boolean | No | Search recursively (default: true) |

## Returns

On success, returns the number of assets found and a list with each asset's path and class.

On error, returns an error message describing why the search failed.

## Example

```json
{
  "class_path": "/Script/Engine.StaticMeshActor",
  "package_path": "/Game/Maps",
  "recursive": true
}
```

## Response

### Success
```
Found 2 assets referencing '/Script/Engine.StaticMeshActor':
- /Game/Maps/MainLevel (World)
- /Game/Maps/TestLevel (World)
```

### Error
```
Failed: Class not found: /Script/Engine.NonExistentClass
```

## Notes

- Only `class_path` is required. The other parameters are optional filters.
- The `class_path` should be in the Unreal Engine object path format (e.g., `/Script/ModuleName.ClassName`).
- Use `package_path` to narrow the search scope to a specific directory.
- By default, the search is recursive and includes all subdirectories of the specified package path.
