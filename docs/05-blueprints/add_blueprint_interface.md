# add_blueprint_interface

Add an interface implementation to a Blueprint.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| blueprint_path | string | Yes | Asset path of the Blueprint |
| interface_path | string | Yes | Class path of the interface, e.g. /Script/MyModule.MyInterface |

## Returns

On success, returns `InterfaceName` of the added interface.

On error, returns a message describing the failure (e.g., Blueprint not found, interface not found, interface already implemented).

## Example

```json
{
  "blueprint_path": "/Game/Blueprints/BP_MyActor",
  "interface_path": "/Script/MyProject.MyInteractableInterface"
}
```

## Response

### Success
```
Interface added successfully.
InterfaceName: MyInteractableInterface
```

### Error
```
Failed to add interface: Interface not found: /Script/MyProject.MyInteractableInterface
```

## Notes

- The `interface_path` must be a fully qualified class path to a valid Blueprint Interface or C++ interface.
- If the Blueprint already implements the specified interface, the operation will fail.
- After adding an interface, function stubs for the interface methods may be added to the Blueprint.
