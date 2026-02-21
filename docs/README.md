# UE-MCP Tools Documentation

This index lists all available tools provided by the UE-MCP plugin, organized by section. Each tool links to its dedicated documentation page.

UE-MCP is an HTTP-based Model Context Protocol (MCP) server running inside Unreal Editor, exposing editor functionality as JSON-RPC 2.0 tools callable by AI assistants and automation clients.

---

## Table of Contents

1. [Actor Management](#01-actor-management)
2. [Level Management](#02-level-management)
3. [Asset Management](#03-asset-management)
4. [Viewport & Camera](#04-viewport--camera)
5. [Blueprints](#05-blueprints)
6. [Materials](#06-materials)
7. [Static Mesh](#07-static-mesh)
8. [Skeletal Mesh & Animation](#08-skeletal-mesh--animation)
9. [Sequencer](#09-sequencer)
10. [Landscape](#10-landscape)
11. [Foliage](#11-foliage)
12. [Navigation & AI](#12-navigation--ai)
13. [Physics](#13-physics)
14. [Build](#14-build)
15. [Console Commands](#15-console-commands)
16. [Project Settings](#16-project-settings)
17. [Source Control](#17-source-control)
18. [UI & Notifications](#18-ui--notifications)
19. [World Partition](#19-world-partition)
20. [Play In Editor](#20-play-in-editor)
21. [Batch Operations](#21-batch-operations)
22. [Blueprint Graph Nodes](#22-blueprint-graph-nodes)
23. [Blueprint Components](#23-blueprint-components)
24. [Blueprint Graph Editing](#24-blueprint-graph-editing)
25. [UMG Widgets](#25-umg-widgets)

---

## 01 Actor Management

- [convert_actor](01-actor-management/convert_actor.md)
- [delete_actor](01-actor-management/delete_actor.md)
- [deselect_all](01-actor-management/deselect_all.md)
- [duplicate_actor](01-actor-management/duplicate_actor.md)
- [get_actor_property](01-actor-management/get_actor_property.md)
- [get_actor_transform](01-actor-management/get_actor_transform.md)
- [get_actors_in_level](01-actor-management/get_actors_in_level.md)
- [group_actors](01-actor-management/group_actors.md)
- [move_actors_to_level](01-actor-management/move_actors_to_level.md)
- [select_actor](01-actor-management/select_actor.md)
- [set_actor_folder](01-actor-management/set_actor_folder.md)
- [set_actor_property](01-actor-management/set_actor_property.md)
- [set_actor_transform](01-actor-management/set_actor_transform.md)
- [spawn_actor](01-actor-management/spawn_actor.md)

## 02 Level Management

- [add_sublevel](02-level-management/add_sublevel.md)
- [load_level](02-level-management/load_level.md)
- [new_level](02-level-management/new_level.md)
- [new_level_from_template](02-level-management/new_level_from_template.md)
- [remove_sublevel](02-level-management/remove_sublevel.md)
- [save_all_dirty_levels](02-level-management/save_all_dirty_levels.md)
- [save_level](02-level-management/save_level.md)
- [set_current_level](02-level-management/set_current_level.md)
- [set_level_visibility](02-level-management/set_level_visibility.md)

## 03 Asset Management

- [create_asset](03-asset-management/create_asset.md)
- [delete_asset](03-asset-management/delete_asset.md)
- [duplicate_asset](03-asset-management/duplicate_asset.md)
- [export_asset](03-asset-management/export_asset.md)
- [find_assets](03-asset-management/find_assets.md)
- [find_referencers_of_class](03-asset-management/find_referencers_of_class.md)
- [get_asset_dependencies](03-asset-management/get_asset_dependencies.md)
- [get_asset_metadata](03-asset-management/get_asset_metadata.md)
- [get_asset_property](03-asset-management/get_asset_property.md)
- [get_asset_referencers](03-asset-management/get_asset_referencers.md)
- [import_asset](03-asset-management/import_asset.md)
- [list_assets](03-asset-management/list_assets.md)
- [load_asset](03-asset-management/load_asset.md)
- [rename_asset](03-asset-management/rename_asset.md)
- [save_asset](03-asset-management/save_asset.md)
- [set_asset_metadata](03-asset-management/set_asset_metadata.md)
- [set_asset_property](03-asset-management/set_asset_property.md)

## 04 Viewport & Camera

- [eject_pilot](04-viewport-camera/eject_pilot.md)
- [focus_on_actors](04-viewport-camera/focus_on_actors.md)
- [get_viewport_camera](04-viewport-camera/get_viewport_camera.md)
- [jump_to_bookmark](04-viewport-camera/jump_to_bookmark.md)
- [pilot_actor](04-viewport-camera/pilot_actor.md)
- [set_bookmark](04-viewport-camera/set_bookmark.md)
- [set_camera_fov](04-viewport-camera/set_camera_fov.md)
- [set_game_view](04-viewport-camera/set_game_view.md)
- [set_view_mode](04-viewport-camera/set_view_mode.md)
- [set_viewport_camera](04-viewport-camera/set_viewport_camera.md)
- [set_viewport_realtime](04-viewport-camera/set_viewport_realtime.md)
- [take_screenshot](04-viewport-camera/take_screenshot.md)

## 05 Blueprints

- [add_blueprint_function](05-blueprints/add_blueprint_function.md)
- [add_blueprint_interface](05-blueprints/add_blueprint_interface.md)
- [add_blueprint_variable](05-blueprints/add_blueprint_variable.md)
- [add_event_dispatcher](05-blueprints/add_event_dispatcher.md)
- [compile_blueprint](05-blueprints/compile_blueprint.md)
- [create_blueprint](05-blueprints/create_blueprint.md)
- [create_blueprint_from_actor](05-blueprints/create_blueprint_from_actor.md)
- [find_function](05-blueprints/find_function.md)
- [get_blueprint_graphs](05-blueprints/get_blueprint_graphs.md)
- [get_blueprint_parent_class](05-blueprints/get_blueprint_parent_class.md)
- [open_blueprint_editor](05-blueprints/open_blueprint_editor.md)
- [remove_blueprint_variable](05-blueprints/remove_blueprint_variable.md)
- [reparent_blueprint](05-blueprints/reparent_blueprint.md)

## 06 Materials

- [connect_material_expressions](06-materials/connect_material_expressions.md)
- [connect_material_property](06-materials/connect_material_property.md)
- [create_material_expression](06-materials/create_material_expression.md)
- [delete_material_expression](06-materials/delete_material_expression.md)
- [get_material_expression_property](06-materials/get_material_expression_property.md)
- [get_material_expressions](06-materials/get_material_expressions.md)
- [get_material_parameter](06-materials/get_material_parameter.md)
- [get_material_statistics](06-materials/get_material_statistics.md)
- [recompile_material](06-materials/recompile_material.md)
- [set_material_expression_property](06-materials/set_material_expression_property.md)
- [set_material_instance_parameter](06-materials/set_material_instance_parameter.md)
- [set_material_instance_parent](06-materials/set_material_instance_parent.md)

## 07 Static Mesh

- [add_uv_channel](07-static-mesh/add_uv_channel.md)
- [generate_uv_channel](07-static-mesh/generate_uv_channel.md)
- [get_lod_settings](07-static-mesh/get_lod_settings.md)
- [get_mesh_bounds](07-static-mesh/get_mesh_bounds.md)
- [import_lod](07-static-mesh/import_lod.md)
- [remove_uv_channel](07-static-mesh/remove_uv_channel.md)
- [set_collision](07-static-mesh/set_collision.md)
- [set_mesh_material](07-static-mesh/set_mesh_material.md)
- [set_static_mesh_lod](07-static-mesh/set_static_mesh_lod.md)

## 08 Skeletal Mesh & Animation

- [apply_anim_compression](08-skeletal-mesh-animation/apply_anim_compression.md)
- [create_anim_asset](08-skeletal-mesh-animation/create_anim_asset.md)
- [create_anim_blueprint](08-skeletal-mesh-animation/create_anim_blueprint.md)
- [get_physics_asset](08-skeletal-mesh-animation/get_physics_asset.md)
- [get_skeleton_info](08-skeletal-mesh-animation/get_skeleton_info.md)
- [regenerate_skeletal_lod](08-skeletal-mesh-animation/regenerate_skeletal_lod.md)
- [reimport_skeletal_mesh](08-skeletal-mesh-animation/reimport_skeletal_mesh.md)
- [set_physics_asset](08-skeletal-mesh-animation/set_physics_asset.md)

## 09 Sequencer

- [sequencer_add_actor](09-sequencer/sequencer_add_actor.md)
- [sequencer_add_marker](09-sequencer/sequencer_add_marker.md)
- [sequencer_add_track](09-sequencer/sequencer_add_track.md)
- [sequencer_force_evaluate](09-sequencer/sequencer_force_evaluate.md)
- [sequencer_get_time](09-sequencer/sequencer_get_time.md)
- [sequencer_pause](09-sequencer/sequencer_pause.md)
- [sequencer_play](09-sequencer/sequencer_play.md)
- [sequencer_set_playback_range](09-sequencer/sequencer_set_playback_range.md)
- [sequencer_set_time](09-sequencer/sequencer_set_time.md)
- [sequencer_stop](09-sequencer/sequencer_stop.md)

## 10 Landscape

- [export_landscape](10-landscape/export_landscape.md)
- [get_landscape_heightmap](10-landscape/get_landscape_heightmap.md)
- [get_landscape_weightmap](10-landscape/get_landscape_weightmap.md)
- [import_landscape](10-landscape/import_landscape.md)
- [rebuild_landscape](10-landscape/rebuild_landscape.md)
- [set_landscape_heightmap](10-landscape/set_landscape_heightmap.md)
- [set_landscape_weightmap](10-landscape/set_landscape_weightmap.md)

## 11 Foliage

- [add_foliage_instances](11-foliage/add_foliage_instances.md)
- [register_foliage_mesh](11-foliage/register_foliage_mesh.md)
- [remove_foliage_instances](11-foliage/remove_foliage_instances.md)
- [resimulate_procedural_foliage](11-foliage/resimulate_procedural_foliage.md)

## 12 Navigation & AI

- [build_navigation](12-navigation-ai/build_navigation.md)
- [find_path](12-navigation-ai/find_path.md)
- [get_random_reachable_point](12-navigation-ai/get_random_reachable_point.md)
- [project_point_to_nav](12-navigation-ai/project_point_to_nav.md)
- [rebuild_navigation](12-navigation-ai/rebuild_navigation.md)

## 13 Physics

- [create_physics_collision](13-physics/create_physics_collision.md)
- [manage_constraints](13-physics/manage_constraints.md)
- [set_physical_material](13-physics/set_physical_material.md)
- [set_physics_velocity](13-physics/set_physics_velocity.md)

## 14 Build

- [build_all](14-build/build_all.md)
- [build_geometry](14-build/build_geometry.md)
- [build_hlod](14-build/build_hlod.md)
- [build_lighting](14-build/build_lighting.md)
- [build_navigation_data](14-build/build_navigation_data.md)
- [build_texture_streaming](14-build/build_texture_streaming.md)
- [get_build_status](14-build/get_build_status.md)

## 15 Console Commands

- [execute_console_command](15-console-commands/execute_console_command.md)
- [get_cvar](15-console-commands/get_cvar.md)
- [set_cvar](15-console-commands/set_cvar.md)

## 16 Project Settings

- [get_config_value](16-project-settings/get_config_value.md)
- [get_project_info](16-project-settings/get_project_info.md)
- [get_project_paths](16-project-settings/get_project_paths.md)
- [set_config_value](16-project-settings/set_config_value.md)

## 17 Source Control

- [scc_add](17-source-control/scc_add.md)
- [scc_checkout](17-source-control/scc_checkout.md)
- [scc_delete](17-source-control/scc_delete.md)
- [scc_revert](17-source-control/scc_revert.md)
- [scc_status](17-source-control/scc_status.md)
- [scc_submit](17-source-control/scc_submit.md)
- [scc_sync](17-source-control/scc_sync.md)

## 18 UI & Notifications

- [close_asset_editor](18-ui-notifications/close_asset_editor.md)
- [get_selected_content_browser](18-ui-notifications/get_selected_content_browser.md)
- [open_asset_editor](18-ui-notifications/open_asset_editor.md)
- [show_dialog](18-ui-notifications/show_dialog.md)
- [show_notification](18-ui-notifications/show_notification.md)
- [sync_content_browser](18-ui-notifications/sync_content_browser.md)

## 19 World Partition

- [build_hlod_wp](19-world-partition/build_hlod_wp.md)
- [delete_hlod_wp](19-world-partition/delete_hlod_wp.md)

## 20 Play In Editor

- [is_playing](20-play-in-editor/is_playing.md)
- [play_in_editor](20-play-in-editor/play_in_editor.md)
- [simulate_in_editor](20-play-in-editor/simulate_in_editor.md)
- [stop_pie](20-play-in-editor/stop_pie.md)

## 21 Batch Operations

- [batch_connect_material_expressions](21-batch-operations/batch_connect_material_expressions.md)
- [batch_set_actor_folders](21-batch-operations/batch_set_actor_folders.md)
- [batch_set_actor_properties](21-batch-operations/batch_set_actor_properties.md)
- [batch_set_actor_transforms](21-batch-operations/batch_set_actor_transforms.md)
- [batch_set_material_expression_properties](21-batch-operations/batch_set_material_expression_properties.md)

## 22 Blueprint Graph Nodes

- [add_comment_box](22-blueprint-graph-nodes/add_comment_box.md)
- [batch_set_node_positions](22-blueprint-graph-nodes/batch_set_node_positions.md)
- [delete_comment_box](22-blueprint-graph-nodes/delete_comment_box.md)
- [get_graph_nodes](22-blueprint-graph-nodes/get_graph_nodes.md)
- [get_graph_nodes_in_area](22-blueprint-graph-nodes/get_graph_nodes_in_area.md)
- [get_graph_nodes_summary](22-blueprint-graph-nodes/get_graph_nodes_summary.md)
- [set_comment_box_properties](22-blueprint-graph-nodes/set_comment_box_properties.md)
- [set_node_position](22-blueprint-graph-nodes/set_node_position.md)

## 23 Blueprint Components

- [add_blueprint_component](23-blueprint-components/add_blueprint_component.md)
- [get_blueprint_component_property](23-blueprint-components/get_blueprint_component_property.md)
- [get_blueprint_components](23-blueprint-components/get_blueprint_components.md)
- [remove_blueprint_component](23-blueprint-components/remove_blueprint_component.md)
- [set_blueprint_component_property](23-blueprint-components/set_blueprint_component_property.md)

## 24 Blueprint Graph Editing

- [add_graph_node](24-blueprint-graph-editing/add_graph_node.md)
- [add_graph_nodes_batch](24-blueprint-graph-editing/add_graph_nodes_batch.md)
- [batch_connect_graph_pins](24-blueprint-graph-editing/batch_connect_graph_pins.md)
- [batch_delete_graph_nodes](24-blueprint-graph-editing/batch_delete_graph_nodes.md)
- [batch_disconnect_graph_pins](24-blueprint-graph-editing/batch_disconnect_graph_pins.md)
- [batch_set_pin_defaults](24-blueprint-graph-editing/batch_set_pin_defaults.md)
- [connect_graph_pins](24-blueprint-graph-editing/connect_graph_pins.md)
- [delete_graph_node](24-blueprint-graph-editing/delete_graph_node.md)
- [disconnect_graph_pins](24-blueprint-graph-editing/disconnect_graph_pins.md)
- [set_pin_default_value](24-blueprint-graph-editing/set_pin_default_value.md)

## 25 UMG Widgets

- [add_widget](25-umg-widgets/add_widget.md)
- [create_widget_blueprint](25-umg-widgets/create_widget_blueprint.md)
- [export_widgets](25-umg-widgets/export_widgets.md)
- [get_widget_animations](25-umg-widgets/get_widget_animations.md)
- [get_widget_property](25-umg-widgets/get_widget_property.md)
- [get_widget_tree](25-umg-widgets/get_widget_tree.md)
- [import_widgets](25-umg-widgets/import_widgets.md)
- [move_widget](25-umg-widgets/move_widget.md)
- [remove_widget](25-umg-widgets/remove_widget.md)
- [rename_widget](25-umg-widgets/rename_widget.md)
- [replace_widget](25-umg-widgets/replace_widget.md)
- [set_widget_property](25-umg-widgets/set_widget_property.md)
- [set_widget_slot](25-umg-widgets/set_widget_slot.md)
