# delete_hlod_wp

Delete HLOD data for World Partition.

## Parameters

This tool takes no parameters.

## Returns

On success, returns a text message confirming the HLOD data was deleted.

On error, returns `isError: true` with an error message describing what went wrong.

## Example

```json
{}
```

## Response

### Success
```
HLOD data deleted
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
HLOD deletion failed
```

## Notes

- This removes all generated HLOD data for the current World Partition level.
- The current level must be a World Partition level for this operation to succeed.
- After deleting HLOD data, you can rebuild it using the `build_hlod_wp` tool.
