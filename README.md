# UE-MCP

MCP (Model Context Protocol) Server plugin for Unreal Engine 5.7. Runs an HTTP server inside the editor, exposing engine functionality via JSON-RPC 2.0.

## Status

- **Transport:** HTTP on `localhost:8080/mcp`
- **Protocol:** MCP 2025-03-26
- [x] Plugin skeleton + HTTP server
- [x] MCP protocol handler (initialize, tools/list, tools/call, ping)
- [x] Session management (Mcp-Session-Id)
- [x] hello_world test tool

> **Full API documentation:** [docs/README.md](docs/README.md) — parameters, return values, examples for every tool.

## Tools Checklist

### 1. Actor Management
- [x] [spawn_actor](docs/01-actor-management/spawn_actor.md) — spawn from class or asset
- [x] [delete_actor](docs/01-actor-management/delete_actor.md) — delete actors
- [x] [duplicate_actor](docs/01-actor-management/duplicate_actor.md) — duplicate actors
- [x] [get_actors_in_level](docs/01-actor-management/get_actors_in_level.md) — list all actors in level
- [x] [select_actor](docs/01-actor-management/select_actor.md) — select actors
- [x] [deselect_all](docs/01-actor-management/deselect_all.md) — deselect all actors
- [x] [get_actor_transform](docs/01-actor-management/get_actor_transform.md) — read actor transform
- [x] [set_actor_transform](docs/01-actor-management/set_actor_transform.md) — set actor transform
- [x] [get_actor_property](docs/01-actor-management/get_actor_property.md) — read actor property
- [x] [set_actor_property](docs/01-actor-management/set_actor_property.md) — set actor property
- [x] [convert_actor](docs/01-actor-management/convert_actor.md) — convert actor type
- [x] [move_actors_to_level](docs/01-actor-management/move_actors_to_level.md) — move actors between levels
- [x] [group_actors](docs/01-actor-management/group_actors.md) — group actors
- [x] [set_actor_folder](docs/01-actor-management/set_actor_folder.md) — manage Outliner folders

### 2. Level Management
- [x] [new_level](docs/02-level-management/new_level.md) — create a new level
- [x] [new_level_from_template](docs/02-level-management/new_level_from_template.md) — create from template
- [x] [load_level](docs/02-level-management/load_level.md) — load a level
- [x] [save_level](docs/02-level-management/save_level.md) — save the current level
- [x] [save_all_dirty_levels](docs/02-level-management/save_all_dirty_levels.md) — save all modified levels
- [x] [add_sublevel](docs/02-level-management/add_sublevel.md) — add a sublevel (streaming)
- [x] [remove_sublevel](docs/02-level-management/remove_sublevel.md) — remove a sublevel
- [x] [set_current_level](docs/02-level-management/set_current_level.md) — switch the current level
- [x] [set_level_visibility](docs/02-level-management/set_level_visibility.md) — toggle level visibility

### 3. Asset Management
- [x] [load_asset](docs/03-asset-management/load_asset.md) — load asset by path
- [x] [create_asset](docs/03-asset-management/create_asset.md) — create a new asset
- [x] [duplicate_asset](docs/03-asset-management/duplicate_asset.md) — duplicate asset
- [x] [rename_asset](docs/03-asset-management/rename_asset.md) — rename asset
- [x] [delete_asset](docs/03-asset-management/delete_asset.md) — delete asset
- [x] [save_asset](docs/03-asset-management/save_asset.md) — save asset
- [x] [find_assets](docs/03-asset-management/find_assets.md) — search by path, class, tags
- [x] [list_assets](docs/03-asset-management/list_assets.md) — list assets in a directory
- [x] [import_asset](docs/03-asset-management/import_asset.md) — import files (FBX, textures, audio)
- [x] [export_asset](docs/03-asset-management/export_asset.md) — export asset
- [x] [get_asset_dependencies](docs/03-asset-management/get_asset_dependencies.md) — asset dependencies
- [x] [get_asset_referencers](docs/03-asset-management/get_asset_referencers.md) — what references this asset
- [x] [get_asset_metadata](docs/03-asset-management/get_asset_metadata.md) — read asset metadata
- [x] [set_asset_metadata](docs/03-asset-management/set_asset_metadata.md) — set asset metadata
- [x] [set_asset_property](docs/03-asset-management/set_asset_property.md) — set UPROPERTY on any asset (SoundWave, Material, DataAsset, etc.)
- [x] [get_asset_property](docs/03-asset-management/get_asset_property.md) — read asset UPROPERTY
- [x] [find_referencers_of_class](docs/03-asset-management/find_referencers_of_class.md) — find assets referencing a given class

### 4. Viewport & Camera
- [x] [get_viewport_camera](docs/04-viewport-camera/get_viewport_camera.md) — read camera position/rotation
- [x] [set_viewport_camera](docs/04-viewport-camera/set_viewport_camera.md) — set camera position/rotation
- [x] [set_game_view](docs/04-viewport-camera/set_game_view.md) — toggle Game View
- [x] [set_viewport_realtime](docs/04-viewport-camera/set_viewport_realtime.md) — toggle Realtime rendering
- [x] [set_view_mode](docs/04-viewport-camera/set_view_mode.md) — view mode (Lit/Wireframe/Unlit)
- [x] [set_camera_fov](docs/04-viewport-camera/set_camera_fov.md) — change field of view
- [x] [focus_on_actors](docs/04-viewport-camera/focus_on_actors.md) — focus on actors
- [x] [take_screenshot](docs/04-viewport-camera/take_screenshot.md) — viewport screenshot
- [x] [pilot_actor](docs/04-viewport-camera/pilot_actor.md) — pilot camera to actor
- [x] [eject_pilot](docs/04-viewport-camera/eject_pilot.md) — eject camera from actor
- [x] [set_bookmark](docs/04-viewport-camera/set_bookmark.md) — save camera bookmark
- [x] [jump_to_bookmark](docs/04-viewport-camera/jump_to_bookmark.md) — jump to bookmark

### 5. Blueprints
- [x] [create_blueprint](docs/05-blueprints/create_blueprint.md) — create Blueprint from class
- [x] [create_blueprint_from_actor](docs/05-blueprints/create_blueprint_from_actor.md) — create from actor
- [x] [compile_blueprint](docs/05-blueprints/compile_blueprint.md) — compile Blueprint
- [x] [add_blueprint_variable](docs/05-blueprints/add_blueprint_variable.md) — add variable
- [x] [remove_blueprint_variable](docs/05-blueprints/remove_blueprint_variable.md) — remove variable
- [x] [add_blueprint_function](docs/05-blueprints/add_blueprint_function.md) — add function
- [x] [add_blueprint_interface](docs/05-blueprints/add_blueprint_interface.md) — add interface
- [x] [get_blueprint_graphs](docs/05-blueprints/get_blueprint_graphs.md) — list graphs
- [x] [reparent_blueprint](docs/05-blueprints/reparent_blueprint.md) — change parent class
- [x] [open_blueprint_editor](docs/05-blueprints/open_blueprint_editor.md) — open in editor
- [x] [add_event_dispatcher](docs/05-blueprints/add_event_dispatcher.md) — create Event Dispatcher with optional parameters
- [x] [get_blueprint_parent_class](docs/05-blueprints/get_blueprint_parent_class.md) — get parent class and Blueprint
- [x] [find_function](docs/05-blueprints/find_function.md) — search Blueprint-callable functions by name

### 6. Materials
- [x] [create_material_expression](docs/06-materials/create_material_expression.md) — create expression node
- [x] [delete_material_expression](docs/06-materials/delete_material_expression.md) — delete expression node
- [x] [connect_material_expressions](docs/06-materials/connect_material_expressions.md) — connect expression nodes
- [x] [connect_material_property](docs/06-materials/connect_material_property.md) — connect to material output
- [x] [recompile_material](docs/06-materials/recompile_material.md) — recompile material
- [x] [get_material_parameter](docs/06-materials/get_material_parameter.md) — read parameter (scalar/vector/texture)
- [x] [set_material_instance_parameter](docs/06-materials/set_material_instance_parameter.md) — set material instance parameter
- [x] [set_material_instance_parent](docs/06-materials/set_material_instance_parent.md) — change material instance parent
- [x] [get_material_statistics](docs/06-materials/get_material_statistics.md) — shader statistics
- [x] [get_material_expressions](docs/06-materials/get_material_expressions.md) — list all expression nodes in material
- [x] [get_material_expression_property](docs/06-materials/get_material_expression_property.md) — read material expression property
- [x] [set_material_expression_property](docs/06-materials/set_material_expression_property.md) — set material expression property (colors, parameters, layers)

### 7. Static Mesh
- [x] [set_static_mesh_lod](docs/07-static-mesh/set_static_mesh_lod.md) — configure LOD settings
- [x] [import_lod](docs/07-static-mesh/import_lod.md) — import LOD from FBX
- [x] [get_lod_settings](docs/07-static-mesh/get_lod_settings.md) — read LOD settings
- [x] [set_collision](docs/07-static-mesh/set_collision.md) — configure collision
- [x] [add_uv_channel](docs/07-static-mesh/add_uv_channel.md) — add UV channel
- [x] [remove_uv_channel](docs/07-static-mesh/remove_uv_channel.md) — remove UV channel
- [x] [generate_uv_channel](docs/07-static-mesh/generate_uv_channel.md) — generate UV (planar/cylindrical/box)
- [x] [set_mesh_material](docs/07-static-mesh/set_mesh_material.md) — assign material by slot
- [x] [get_mesh_bounds](docs/07-static-mesh/get_mesh_bounds.md) — bounding box

### 8. Skeletal Mesh & Animation
- [x] [regenerate_skeletal_lod](docs/08-skeletal-mesh-animation/regenerate_skeletal_lod.md) — regenerate LOD
- [x] [get_skeleton_info](docs/08-skeletal-mesh-animation/get_skeleton_info.md) — bone count, vertex count
- [x] [get_physics_asset](docs/08-skeletal-mesh-animation/get_physics_asset.md) — read Physics Asset
- [x] [set_physics_asset](docs/08-skeletal-mesh-animation/set_physics_asset.md) — set Physics Asset
- [x] [reimport_skeletal_mesh](docs/08-skeletal-mesh-animation/reimport_skeletal_mesh.md) — reimport
- [x] [create_anim_asset](docs/08-skeletal-mesh-animation/create_anim_asset.md) — create AnimSequence/BlendSpace/Montage
- [x] [create_anim_blueprint](docs/08-skeletal-mesh-animation/create_anim_blueprint.md) — create Anim Blueprint
- [x] [apply_anim_compression](docs/08-skeletal-mesh-animation/apply_anim_compression.md) — apply animation compression

### 9. Sequencer / Cinematics
- [x] [sequencer_play](docs/09-sequencer/sequencer_play.md) — play sequence
- [x] [sequencer_pause](docs/09-sequencer/sequencer_pause.md) — pause playback
- [x] [sequencer_stop](docs/09-sequencer/sequencer_stop.md) — stop playback
- [x] [sequencer_set_time](docs/09-sequencer/sequencer_set_time.md) — set timeline position
- [x] [sequencer_get_time](docs/09-sequencer/sequencer_get_time.md) — get timeline position
- [x] [sequencer_set_playback_range](docs/09-sequencer/sequencer_set_playback_range.md) — set playback range
- [x] [sequencer_add_actor](docs/09-sequencer/sequencer_add_actor.md) — add actor to sequence
- [x] [sequencer_add_track](docs/09-sequencer/sequencer_add_track.md) — add track
- [x] [sequencer_add_marker](docs/09-sequencer/sequencer_add_marker.md) — add marker
- [x] [sequencer_force_evaluate](docs/09-sequencer/sequencer_force_evaluate.md) — force evaluation

### 10. Landscape
- [x] [get_landscape_heightmap](docs/10-landscape/get_landscape_heightmap.md) — read heightmap
- [x] [set_landscape_heightmap](docs/10-landscape/set_landscape_heightmap.md) — write heightmap
- [x] [get_landscape_weightmap](docs/10-landscape/get_landscape_weightmap.md) — read weight map
- [x] [set_landscape_weightmap](docs/10-landscape/set_landscape_weightmap.md) — write weight map
- [x] [import_landscape](docs/10-landscape/import_landscape.md) — import landscape
- [x] [export_landscape](docs/10-landscape/export_landscape.md) — export landscape
- [x] [rebuild_landscape](docs/10-landscape/rebuild_landscape.md) — rebuild landscape data

### 11. Foliage
- [x] [add_foliage_instances](docs/11-foliage/add_foliage_instances.md) — add foliage instances
- [x] [remove_foliage_instances](docs/11-foliage/remove_foliage_instances.md) — remove foliage instances
- [x] [register_foliage_mesh](docs/11-foliage/register_foliage_mesh.md) — register foliage mesh
- [x] [resimulate_procedural_foliage](docs/11-foliage/resimulate_procedural_foliage.md) — procedural foliage generation

### 12. Navigation / AI
- [x] [build_navigation](docs/12-navigation-ai/build_navigation.md) — build NavMesh
- [x] [rebuild_navigation](docs/12-navigation-ai/rebuild_navigation.md) — rebuild NavMesh
- [x] [find_path](docs/12-navigation-ai/find_path.md) — find path between points
- [x] [project_point_to_nav](docs/12-navigation-ai/project_point_to_nav.md) — project point onto NavMesh
- [x] [get_random_reachable_point](docs/12-navigation-ai/get_random_reachable_point.md) — random reachable position

### 13. Physics
- [x] [create_physics_collision](docs/13-physics/create_physics_collision.md) — configure BodySetup collision
- [x] [set_physical_material](docs/13-physics/set_physical_material.md) — set physical material
- [x] [set_physics_velocity](docs/13-physics/set_physics_velocity.md) — set velocity (linear/angular)
- [x] [manage_constraints](docs/13-physics/manage_constraints.md) — manage physics constraints

### 14. Build
- [x] [build_geometry](docs/14-build/build_geometry.md) — build geometry
- [x] [build_lighting](docs/14-build/build_lighting.md) — build lighting
- [x] [build_navigation_data](docs/14-build/build_navigation_data.md) — build AI paths
- [x] [build_hlod](docs/14-build/build_hlod.md) — build HLOD
- [x] [build_texture_streaming](docs/14-build/build_texture_streaming.md) — build texture streaming
- [x] [build_all](docs/14-build/build_all.md) — full build
- [x] [get_build_status](docs/14-build/get_build_status.md) — current build status

### 15. Console Commands
- [x] [execute_console_command](docs/15-console-commands/execute_console_command.md) — execute console command
- [x] [get_cvar](docs/15-console-commands/get_cvar.md) — read console variable
- [x] [set_cvar](docs/15-console-commands/set_cvar.md) — set console variable

### 16. Project Settings
- [x] [get_project_info](docs/16-project-settings/get_project_info.md) — project information
- [x] [get_config_value](docs/16-project-settings/get_config_value.md) — read from config
- [x] [set_config_value](docs/16-project-settings/set_config_value.md) — write to config
- [x] [get_project_paths](docs/16-project-settings/get_project_paths.md) — project paths (Content, Config, Plugins)

### 17. Source Control
- [x] [scc_checkout](docs/17-source-control/scc_checkout.md) — checkout files
- [x] [scc_add](docs/17-source-control/scc_add.md) — add files
- [x] [scc_delete](docs/17-source-control/scc_delete.md) — delete files
- [x] [scc_revert](docs/17-source-control/scc_revert.md) — revert changes
- [x] [scc_status](docs/17-source-control/scc_status.md) — file status
- [x] [scc_submit](docs/17-source-control/scc_submit.md) — commit changes
- [x] [scc_sync](docs/17-source-control/scc_sync.md) — sync files

### 18. UI & Notifications
- [x] [show_notification](docs/18-ui-notifications/show_notification.md) — toast notification in editor
- [x] [show_dialog](docs/18-ui-notifications/show_dialog.md) — modal dialog
- [x] [open_asset_editor](docs/18-ui-notifications/open_asset_editor.md) — open asset editor
- [x] [close_asset_editor](docs/18-ui-notifications/close_asset_editor.md) — close asset editor
- [x] [sync_content_browser](docs/18-ui-notifications/sync_content_browser.md) — navigate in Content Browser
- [x] [get_selected_content_browser](docs/18-ui-notifications/get_selected_content_browser.md) — current Content Browser selection

### 19. World Partition
- [x] [build_hlod_wp](docs/19-world-partition/build_hlod_wp.md) — build HLOD for World Partition
- [x] [delete_hlod_wp](docs/19-world-partition/delete_hlod_wp.md) — delete HLOD

### 20. Play In Editor (PIE)
- [x] [play_in_editor](docs/20-play-in-editor/play_in_editor.md) — start PIE
- [x] [stop_pie](docs/20-play-in-editor/stop_pie.md) — stop PIE
- [x] [simulate_in_editor](docs/20-play-in-editor/simulate_in_editor.md) — Simulate mode
- [x] [is_playing](docs/20-play-in-editor/is_playing.md) — check PIE status

### 21. Batch Operations
- [x] [batch_set_actor_folders](docs/21-batch-operations/batch_set_actor_folders.md) — batch set actor Outliner folders (up to 100 per call)
- [x] [batch_set_actor_properties](docs/21-batch-operations/batch_set_actor_properties.md) — batch set actor properties (up to 100 per call)
- [x] [batch_set_material_expression_properties](docs/21-batch-operations/batch_set_material_expression_properties.md) — batch set material expression properties (up to 100 per call)
- [x] [batch_connect_material_expressions](docs/21-batch-operations/batch_connect_material_expressions.md) — batch connect material expressions (up to 100 per call)
- [x] [batch_set_actor_transforms](docs/21-batch-operations/batch_set_actor_transforms.md) — batch set actor transforms (up to 100 per call)

### 22. Blueprint Graph Nodes
- [x] [get_graph_nodes](docs/22-blueprint-graph-nodes/get_graph_nodes.md) — read all nodes in a graph (node_id, class, title, position, size, pins, comment)
- [x] [set_node_position](docs/22-blueprint-graph-nodes/set_node_position.md) — move node on graph
- [x] [batch_set_node_positions](docs/22-blueprint-graph-nodes/batch_set_node_positions.md) — batch move nodes (up to 100 per call)
- [x] [add_comment_box](docs/22-blueprint-graph-nodes/add_comment_box.md) — add Comment Box (EdGraphNode_Comment)
- [x] [delete_comment_box](docs/22-blueprint-graph-nodes/delete_comment_box.md) — delete Comment Box
- [x] [set_comment_box_properties](docs/22-blueprint-graph-nodes/set_comment_box_properties.md) — modify Comment Box properties (text, color, position, size)
- [x] [get_graph_nodes_summary](docs/22-blueprint-graph-nodes/get_graph_nodes_summary.md) — lightweight node list (id, class, title, connected_node_ids) with optional class_filter
- [x] [get_graph_nodes_in_area](docs/22-blueprint-graph-nodes/get_graph_nodes_in_area.md) — spatial query for nodes in a rectangular area (collision detection for placement)

### 23. Blueprint Components
- [x] [add_blueprint_component](docs/23-blueprint-components/add_blueprint_component.md) — add component to Blueprint (class, name, parent)
- [x] [remove_blueprint_component](docs/23-blueprint-components/remove_blueprint_component.md) — remove component from Blueprint
- [x] [get_blueprint_components](docs/23-blueprint-components/get_blueprint_components.md) — list all Blueprint components (name, class, parent, is_root)
- [x] [set_blueprint_component_property](docs/23-blueprint-components/set_blueprint_component_property.md) — set component property (default value)
- [x] [get_blueprint_component_property](docs/23-blueprint-components/get_blueprint_component_property.md) — read component property

### 24. Blueprint Graph Editing
- [x] [add_graph_node](docs/24-blueprint-graph-editing/add_graph_node.md) — add node to graph (CallFunction, Event, ComponentBoundEvent, VariableGet, VariableSet, DynamicCast, IfThenElse/Branch, MacroInstance, SwitchEnum, MapForEach, FormatText, EnumToString)
- [x] [connect_graph_pins](docs/24-blueprint-graph-editing/connect_graph_pins.md) — connect pins between nodes
- [x] [disconnect_graph_pins](docs/24-blueprint-graph-editing/disconnect_graph_pins.md) — disconnect connected pins
- [x] [set_pin_default_value](docs/24-blueprint-graph-editing/set_pin_default_value.md) — set default value on a pin
- [x] [delete_graph_node](docs/24-blueprint-graph-editing/delete_graph_node.md) — delete node from graph
- [x] [add_graph_nodes_batch](docs/24-blueprint-graph-editing/add_graph_nodes_batch.md) — batch create nodes + connections in a single call
- [x] [batch_connect_graph_pins](docs/24-blueprint-graph-editing/batch_connect_graph_pins.md) — batch connect pins (up to 100 per call)
- [x] [batch_set_pin_defaults](docs/24-blueprint-graph-editing/batch_set_pin_defaults.md) — batch set pin defaults (up to 100 per call)
- [x] [batch_delete_graph_nodes](docs/24-blueprint-graph-editing/batch_delete_graph_nodes.md) — batch delete nodes (up to 100 per call)
- [x] [batch_disconnect_graph_pins](docs/24-blueprint-graph-editing/batch_disconnect_graph_pins.md) — batch disconnect pins (up to 100 per call)

### 25. UMG Widgets
- [x] [create_widget_blueprint](docs/25-umg-widgets/create_widget_blueprint.md) — create Widget Blueprint with root widget
- [x] [get_widget_tree](docs/25-umg-widgets/get_widget_tree.md) — get widget tree
- [x] [add_widget](docs/25-umg-widgets/add_widget.md) — add widget to panel
- [x] [remove_widget](docs/25-umg-widgets/remove_widget.md) — remove widget from tree
- [x] [move_widget](docs/25-umg-widgets/move_widget.md) — move widget to another container
- [x] [rename_widget](docs/25-umg-widgets/rename_widget.md) — rename widget
- [x] [replace_widget](docs/25-umg-widgets/replace_widget.md) — replace widget type preserving children
- [x] [set_widget_property](docs/25-umg-widgets/set_widget_property.md) — set widget property (Text, Color, Visibility, etc.)
- [x] [get_widget_property](docs/25-umg-widgets/get_widget_property.md) — read widget property
- [x] [set_widget_slot](docs/25-umg-widgets/set_widget_slot.md) — configure slot (anchors, padding, alignment, size)
- [x] [get_widget_animations](docs/25-umg-widgets/get_widget_animations.md) — list Widget Blueprint animations
- [x] [export_widgets](docs/25-umg-widgets/export_widgets.md) — export widgets to text
- [x] [import_widgets](docs/25-umg-widgets/import_widgets.md) — import widgets from text
