# build_hlod_wp

Build HLOD for World Partition.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a text message confirming the HLOD build completed.

On error, returns `isError: true` with an error message describing what went wrong.

## Example

```json
{}
```

## Response

### Success
```
HLOD build completed
```

### Error
```
No editor world available
```
```
Current world is not a World Partition world
```
```
WorldPartitionHLODsBuilder class not found
```
```
HLOD build failed
```

## Notes

- HLOD (Hierarchical Level of Detail) is used with World Partition to optimize rendering of large open worlds.
- The current level must be a World Partition level for this operation to succeed.
- This operation can take a significant amount of time depending on the complexity of the world.
