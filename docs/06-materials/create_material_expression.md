# create_material_expression

Create a material expression node in a material.

## Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| material_path | string | Yes | Asset path of the material |
| expression_class | string | Yes | Expression class name, e.g. Constant, Constant3Vector, ScalarParameter, VectorParameter, TextureSample, Add, Multiply |
| node_pos_x | number | No | X position of the node in the material editor (default: 0) |
| node_pos_y | number | No | Y position of the node in the material editor (default: 0) |

## Returns

On success, returns `ExpressionName`, `ExpressionClass`, and `ExpressionIndex` of the created node.

On error, returns a message describing the failure (e.g., material not found, invalid expression class).

## Example

```json
{
  "material_path": "/Game/Materials/M_Base",
  "expression_class": "Constant3Vector",
  "node_pos_x": -300,
  "node_pos_y": 0
}
```

## Response

### Success
```
Material expression created successfully.
ExpressionName: MaterialExpressionConstant3Vector_0
ExpressionClass: MaterialExpressionConstant3Vector
ExpressionIndex: 3
```

### Error
```
Failed to create material expression: Material not found: /Game/Materials/M_Missing
```

## Notes

- Common expression class names include: `Constant`, `Constant2Vector`, `Constant3Vector`, `Constant4Vector`, `ScalarParameter`, `VectorParameter`, `TextureSample`, `TextureObject`, `Add`, `Multiply`, `Lerp`, `Power`, `TexCoord`, `Time`, `Panner`, `ComponentMask`.
- The `ExpressionIndex` returned can be used with other material tools to connect, delete, or modify the expression.
- Node positions (`node_pos_x`, `node_pos_y`) control the visual layout in the material editor graph.
