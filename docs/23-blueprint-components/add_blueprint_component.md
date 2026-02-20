# add_blueprint_component

Add a component to a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| component_class | string | Yes | Class of the component to add (e.g. StaticMeshComponent, PointLightComponent) |
| component_name | string | No | Optional name for the component. Auto-generated if omitted. |
| parent_component | string | No | Optional name of the parent component to attach to |

## Returns

On success, returns a text message confirming the component was added, including its name, class, and parent (e.g. `"Component 'MyLight' (class: PointLightComponent) added to blueprint, parent: DefaultSceneRoot"`).

On error, returns a message describing the failure.

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "component_class": "PointLightComponent",
  "component_name": "MyLight",
  "parent_component": "DefaultSceneRoot"
}
```

## Notes

- If `component_name` is not provided, UE will auto-generate a unique name.
- If `parent_component` is not provided, the component is attached to the default parent.
- Common component classes: `StaticMeshComponent`, `PointLightComponent`, `BoxCollisionComponent`, `SphereComponent`, `AudioComponent`, `ParticleSystemComponent`, `CameraComponent`.
