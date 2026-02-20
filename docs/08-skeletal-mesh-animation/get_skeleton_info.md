# get_skeleton_info

Get skeleton information: bone hierarchy, vertex count per LOD.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| mesh_path | string | Yes | Asset path of the skeletal mesh |

## Returns

On success, returns a JSON object containing:
- **skeleton** -- name of the skeleton asset
- **num_bones** -- total number of bones
- **num_raw_bones** -- number of raw (non-virtual) bones
- **lod_count** -- number of LOD levels
- **vertex_count_per_lod** -- array of vertex counts, one per LOD level
- **bones** -- array of bone objects, each with:
  - **index** -- bone index
  - **name** -- bone name
  - **parent_index** -- parent bone index (-1 for root)

On error, returns an error message describing the failure reason (e.g., mesh not found).

## Example

```json
{
  "mesh_path": "/Game/Characters/SK_Mannequin"
}
```

## Notes

- This is a read-only tool that does not modify the mesh or skeleton.
- The bone hierarchy can be reconstructed from the `parent_index` fields. A `parent_index` of -1 indicates a root bone.
- The response is returned as a serialized JSON object within the text content.
