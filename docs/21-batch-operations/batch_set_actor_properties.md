# batch_set_actor_properties

Set properties on multiple actors in one call. Up to 100 operations per batch.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| operations | array | Yes | Array of operations. Each has actor_identifier, property_name, and property_value. |

Each object in the `operations` array has the following fields:

| Field | Type | Description |
|-------|------|-------------|
| actor_identifier | string | Identifier of the actor (name or label) |
| property_name | string | Name of the property to set |
| property_value | string | Value to assign to the property |

## Returns

On success, returns a text message with the number of succeeded and failed operations (e.g. `"Batch completed: 5 succeeded, 1 failed"`). The `isError` flag is `true` only when all operations fail.

On error (missing or invalid parameters), returns an error message describing the issue.

If any individual operations fail, the errors are listed with their index and error message.

## Example

```json
{
  "operations": [
    {
      "actor_identifier": "Cube_1",
      "property_name": "bHidden",
      "property_value": "true"
    },
    {
      "actor_identifier": "PointLight_0",
      "property_name": "Intensity",
      "property_value": "5000.0"
    }
  ]
}
```

## Response

### Success
```
Batch completed: 2 succeeded, 0 failed
```

### Error
```
Missing required parameter: operations (array)
```
```
Too many operations. Maximum 100 per batch.
```
```
Batch completed: 1 succeeded, 1 failed

Errors:
- [1] Actor not found: NonExistentActor
```

## Notes

- Maximum of 100 operations per batch. Exceeding this limit returns an error.
- Each operation is executed independently; a failure in one does not stop the others.
- The `isError` flag is only set to `true` when all operations fail (succeeded == 0 and failed > 0).
- Property values are passed as strings and converted internally.
