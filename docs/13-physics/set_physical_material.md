# set_physical_material

Set the physical material override on an actor's primitive component.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| actor_name | string | Yes | Name of the actor |
| material_path | string | Yes | Asset path of the physical material |

## Returns

On success, returns a confirmation message that the physical material was set on the actor.

On error, returns an error message describing the failure (e.g. actor not found, material not found).

## Example

```json
{
  "actor_name": "StaticMeshActor_0",
  "material_path": "/Game/PhysicalMaterials/PM_Ice"
}
```

## Response

### Success
```
Physical material set successfully on actor: StaticMeshActor_0
```

### Error
```
Missing required parameter: actor_name
```
```
Missing required parameter: material_path
```
```
Failed to set physical material: Actor 'InvalidActor' not found
```

## Notes

- The physical material must be a valid `UPhysicalMaterial` asset in the project.
- This sets a material override on the actor's primitive component, which takes priority over any material-level physical material assignments.
- Physical materials affect friction, restitution, density, and other physics properties.
