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

### 1. Управление акторами
- [x] [spawn_actor](docs/01-actor-management/spawn_actor.md) — спавн из класса или ассета
- [x] [delete_actor](docs/01-actor-management/delete_actor.md) — удаление акторов
- [x] [duplicate_actor](docs/01-actor-management/duplicate_actor.md) — дублирование
- [x] [get_actors_in_level](docs/01-actor-management/get_actors_in_level.md) — список всех акторов на уровне
- [x] [select_actor](docs/01-actor-management/select_actor.md) — выделение акторов
- [x] [deselect_all](docs/01-actor-management/deselect_all.md) — снятие выделения
- [x] [get_actor_transform](docs/01-actor-management/get_actor_transform.md) — чтение трансформа
- [x] [set_actor_transform](docs/01-actor-management/set_actor_transform.md) — установка трансформа
- [x] [get_actor_property](docs/01-actor-management/get_actor_property.md) — чтение свойств актора
- [x] [set_actor_property](docs/01-actor-management/set_actor_property.md) — установка свойств актора
- [x] [convert_actor](docs/01-actor-management/convert_actor.md) — конвертация типа актора
- [x] [move_actors_to_level](docs/01-actor-management/move_actors_to_level.md) — перемещение между уровнями
- [x] [group_actors](docs/01-actor-management/group_actors.md) — группировка акторов
- [x] [set_actor_folder](docs/01-actor-management/set_actor_folder.md) — управление папками в Outliner

### 2. Управление уровнями
- [x] [new_level](docs/02-level-management/new_level.md) — создание нового уровня
- [x] [new_level_from_template](docs/02-level-management/new_level_from_template.md) — создание из шаблона
- [x] [load_level](docs/02-level-management/load_level.md) — загрузка уровня
- [x] [save_level](docs/02-level-management/save_level.md) — сохранение текущего уровня
- [x] [save_all_dirty_levels](docs/02-level-management/save_all_dirty_levels.md) — сохранение всех изменённых
- [x] [add_sublevel](docs/02-level-management/add_sublevel.md) — добавление сабуровня (streaming)
- [x] [remove_sublevel](docs/02-level-management/remove_sublevel.md) — удаление сабуровня
- [x] [set_current_level](docs/02-level-management/set_current_level.md) — переключение текущего уровня
- [x] [set_level_visibility](docs/02-level-management/set_level_visibility.md) — управление видимостью уровня

### 3. Работа с ассетами
- [x] [load_asset](docs/03-asset-management/load_asset.md) — загрузка ассета по пути
- [x] [create_asset](docs/03-asset-management/create_asset.md) — создание нового ассета
- [x] [duplicate_asset](docs/03-asset-management/duplicate_asset.md) — дублирование
- [x] [rename_asset](docs/03-asset-management/rename_asset.md) — переименование
- [x] [delete_asset](docs/03-asset-management/delete_asset.md) — удаление
- [x] [save_asset](docs/03-asset-management/save_asset.md) — сохранение
- [x] [find_assets](docs/03-asset-management/find_assets.md) — поиск по пути, классу, тегам
- [x] [list_assets](docs/03-asset-management/list_assets.md) — список ассетов в директории
- [x] [import_asset](docs/03-asset-management/import_asset.md) — импорт файлов (FBX, текстуры, аудио)
- [x] [export_asset](docs/03-asset-management/export_asset.md) — экспорт
- [x] [get_asset_dependencies](docs/03-asset-management/get_asset_dependencies.md) — зависимости ассета
- [x] [get_asset_referencers](docs/03-asset-management/get_asset_referencers.md) — кто ссылается на ассет
- [x] [get_asset_metadata](docs/03-asset-management/get_asset_metadata.md) — чтение метаданных
- [x] [set_asset_metadata](docs/03-asset-management/set_asset_metadata.md) — установка метаданных
- [x] [set_asset_property](docs/03-asset-management/set_asset_property.md) — установка UPROPERTY на любом ассете (SoundWave, Material, DataAsset и т.д.)
- [x] [get_asset_property](docs/03-asset-management/get_asset_property.md) — чтение UPROPERTY ассета
- [x] [find_referencers_of_class](docs/03-asset-management/find_referencers_of_class.md) — поиск ассетов, ссылающихся на указанный класс

### 4. Вьюпорт и камера
- [x] [get_viewport_camera](docs/04-viewport-camera/get_viewport_camera.md) — чтение позиции/ротации камеры
- [x] [set_viewport_camera](docs/04-viewport-camera/set_viewport_camera.md) — установка позиции/ротации
- [x] [set_game_view](docs/04-viewport-camera/set_game_view.md) — переключение Game View
- [x] [set_viewport_realtime](docs/04-viewport-camera/set_viewport_realtime.md) — переключение Realtime
- [x] [set_view_mode](docs/04-viewport-camera/set_view_mode.md) — режим (Lit/Wireframe/Unlit)
- [x] [set_camera_fov](docs/04-viewport-camera/set_camera_fov.md) — изменение FOV
- [x] [focus_on_actors](docs/04-viewport-camera/focus_on_actors.md) — фокус на акторах
- [x] [take_screenshot](docs/04-viewport-camera/take_screenshot.md) — скриншот вьюпорта
- [x] [pilot_actor](docs/04-viewport-camera/pilot_actor.md) — привязка камеры к актору
- [x] [eject_pilot](docs/04-viewport-camera/eject_pilot.md) — отвязка камеры
- [x] [set_bookmark](docs/04-viewport-camera/set_bookmark.md) — сохранение закладки камеры
- [x] [jump_to_bookmark](docs/04-viewport-camera/jump_to_bookmark.md) — переход к закладке

### 5. Blueprints
- [x] [create_blueprint](docs/05-blueprints/create_blueprint.md) — создание Blueprint из класса
- [x] [create_blueprint_from_actor](docs/05-blueprints/create_blueprint_from_actor.md) — создание из актора
- [x] [compile_blueprint](docs/05-blueprints/compile_blueprint.md) — компиляция
- [x] [add_blueprint_variable](docs/05-blueprints/add_blueprint_variable.md) — добавление переменной
- [x] [remove_blueprint_variable](docs/05-blueprints/remove_blueprint_variable.md) — удаление переменной
- [x] [add_blueprint_function](docs/05-blueprints/add_blueprint_function.md) — добавление функции
- [x] [add_blueprint_interface](docs/05-blueprints/add_blueprint_interface.md) — добавление интерфейса
- [x] [get_blueprint_graphs](docs/05-blueprints/get_blueprint_graphs.md) — список графов
- [x] [reparent_blueprint](docs/05-blueprints/reparent_blueprint.md) — смена родительского класса
- [x] [open_blueprint_editor](docs/05-blueprints/open_blueprint_editor.md) — открытие в редакторе
- [x] [add_event_dispatcher](docs/05-blueprints/add_event_dispatcher.md) — создание Event Dispatcher с опциональными параметрами
- [x] [get_blueprint_parent_class](docs/05-blueprints/get_blueprint_parent_class.md) — получение родительского класса и BP

### 6. Материалы
- [x] [create_material_expression](docs/06-materials/create_material_expression.md) — создание ноды
- [x] [delete_material_expression](docs/06-materials/delete_material_expression.md) — удаление ноды
- [x] [connect_material_expressions](docs/06-materials/connect_material_expressions.md) — соединение нод
- [x] [connect_material_property](docs/06-materials/connect_material_property.md) — подключение к выходу
- [x] [recompile_material](docs/06-materials/recompile_material.md) — рекомпиляция
- [x] [get_material_parameter](docs/06-materials/get_material_parameter.md) — чтение параметра (scalar/vector/texture)
- [x] [set_material_instance_parameter](docs/06-materials/set_material_instance_parameter.md) — установка параметра MI
- [x] [set_material_instance_parent](docs/06-materials/set_material_instance_parent.md) — смена родителя MI
- [x] [get_material_statistics](docs/06-materials/get_material_statistics.md) — статистика шейдера
- [x] [get_material_expressions](docs/06-materials/get_material_expressions.md) — список всех expression-нод в материале
- [x] [get_material_expression_property](docs/06-materials/get_material_expression_property.md) — чтение свойства ноды материала
- [x] [set_material_expression_property](docs/06-materials/set_material_expression_property.md) — установка свойства ноды материала (цвета, параметры, слои)

### 7. Static Mesh
- [x] [set_static_mesh_lod](docs/07-static-mesh/set_static_mesh_lod.md) — настройка LOD
- [x] [import_lod](docs/07-static-mesh/import_lod.md) — импорт LOD из FBX
- [x] [get_lod_settings](docs/07-static-mesh/get_lod_settings.md) — чтение настроек LOD
- [x] [set_collision](docs/07-static-mesh/set_collision.md) — настройка коллизий
- [x] [add_uv_channel](docs/07-static-mesh/add_uv_channel.md) — добавление UV канала
- [x] [remove_uv_channel](docs/07-static-mesh/remove_uv_channel.md) — удаление UV канала
- [x] [generate_uv_channel](docs/07-static-mesh/generate_uv_channel.md) — генерация UV (planar/cylindrical/box)
- [x] [set_mesh_material](docs/07-static-mesh/set_mesh_material.md) — назначение материала по слоту
- [x] [get_mesh_bounds](docs/07-static-mesh/get_mesh_bounds.md) — bounding box

### 8. Skeletal Mesh и анимация
- [x] [regenerate_skeletal_lod](docs/08-skeletal-mesh-animation/regenerate_skeletal_lod.md) — регенерация LOD
- [x] [get_skeleton_info](docs/08-skeletal-mesh-animation/get_skeleton_info.md) — bone count, vertex count
- [x] [get_physics_asset](docs/08-skeletal-mesh-animation/get_physics_asset.md) — чтение Physics Asset
- [x] [set_physics_asset](docs/08-skeletal-mesh-animation/set_physics_asset.md) — установка Physics Asset
- [x] [reimport_skeletal_mesh](docs/08-skeletal-mesh-animation/reimport_skeletal_mesh.md) — реимпорт
- [x] [create_anim_asset](docs/08-skeletal-mesh-animation/create_anim_asset.md) — создание AnimSequence/BlendSpace/Montage
- [x] [create_anim_blueprint](docs/08-skeletal-mesh-animation/create_anim_blueprint.md) — создание Anim Blueprint
- [x] [apply_anim_compression](docs/08-skeletal-mesh-animation/apply_anim_compression.md) — применение компрессии

### 9. Секвенсер / Кинематика
- [x] [sequencer_play](docs/09-sequencer/sequencer_play.md) — воспроизведение
- [x] [sequencer_pause](docs/09-sequencer/sequencer_pause.md) — пауза
- [x] [sequencer_stop](docs/09-sequencer/sequencer_stop.md) — остановка
- [x] [sequencer_set_time](docs/09-sequencer/sequencer_set_time.md) — установка позиции на таймлайне
- [x] [sequencer_get_time](docs/09-sequencer/sequencer_get_time.md) — чтение позиции
- [x] [sequencer_set_playback_range](docs/09-sequencer/sequencer_set_playback_range.md) — диапазон воспроизведения
- [x] [sequencer_add_actor](docs/09-sequencer/sequencer_add_actor.md) — добавление актора
- [x] [sequencer_add_track](docs/09-sequencer/sequencer_add_track.md) — добавление трека
- [x] [sequencer_add_marker](docs/09-sequencer/sequencer_add_marker.md) — добавление маркера
- [x] [sequencer_force_evaluate](docs/09-sequencer/sequencer_force_evaluate.md) — принудительная оценка

### 10. Ландшафт
- [x] [get_landscape_heightmap](docs/10-landscape/get_landscape_heightmap.md) — чтение heightmap
- [x] [set_landscape_heightmap](docs/10-landscape/set_landscape_heightmap.md) — запись heightmap
- [x] [get_landscape_weightmap](docs/10-landscape/get_landscape_weightmap.md) — чтение weight map
- [x] [set_landscape_weightmap](docs/10-landscape/set_landscape_weightmap.md) — запись weight map
- [x] [import_landscape](docs/10-landscape/import_landscape.md) — импорт
- [x] [export_landscape](docs/10-landscape/export_landscape.md) — экспорт
- [x] [rebuild_landscape](docs/10-landscape/rebuild_landscape.md) — перестройка данных

### 11. Фолиаж
- [x] [add_foliage_instances](docs/11-foliage/add_foliage_instances.md) — добавление инстансов
- [x] [remove_foliage_instances](docs/11-foliage/remove_foliage_instances.md) — удаление инстансов
- [x] [register_foliage_mesh](docs/11-foliage/register_foliage_mesh.md) — регистрация меша
- [x] [resimulate_procedural_foliage](docs/11-foliage/resimulate_procedural_foliage.md) — процедурная генерация

### 12. Навигация / AI
- [x] [build_navigation](docs/12-navigation-ai/build_navigation.md) — построение NavMesh
- [x] [rebuild_navigation](docs/12-navigation-ai/rebuild_navigation.md) — перестройка
- [x] [find_path](docs/12-navigation-ai/find_path.md) — поиск пути между точками
- [x] [project_point_to_nav](docs/12-navigation-ai/project_point_to_nav.md) — проецирование на NavMesh
- [x] [get_random_reachable_point](docs/12-navigation-ai/get_random_reachable_point.md) — случайная достижимая позиция

### 13. Физика
- [x] [create_physics_collision](docs/13-physics/create_physics_collision.md) — настройка коллизий BodySetup
- [x] [set_physical_material](docs/13-physics/set_physical_material.md) — настройка физического материала
- [x] [set_physics_velocity](docs/13-physics/set_physics_velocity.md) — установка скорости (linear/angular)
- [x] [manage_constraints](docs/13-physics/manage_constraints.md) — управление constraint'ами

### 14. Сборка (Build)
- [x] [build_geometry](docs/14-build/build_geometry.md) — сборка геометрии
- [x] [build_lighting](docs/14-build/build_lighting.md) — сборка освещения
- [x] [build_navigation_data](docs/14-build/build_navigation_data.md) — сборка AI Paths
- [x] [build_hlod](docs/14-build/build_hlod.md) — сборка HLOD
- [x] [build_texture_streaming](docs/14-build/build_texture_streaming.md) — сборка Texture Streaming
- [x] [build_all](docs/14-build/build_all.md) — полная сборка
- [x] [get_build_status](docs/14-build/get_build_status.md) — статус текущей сборки

### 15. Консольные команды
- [x] [execute_console_command](docs/15-console-commands/execute_console_command.md) — выполнение консольной команды
- [x] [get_cvar](docs/15-console-commands/get_cvar.md) — чтение console variable
- [x] [set_cvar](docs/15-console-commands/set_cvar.md) — установка console variable

### 16. Настройки проекта
- [x] [get_project_info](docs/16-project-settings/get_project_info.md) — информация о проекте
- [x] [get_config_value](docs/16-project-settings/get_config_value.md) — чтение из config
- [x] [set_config_value](docs/16-project-settings/set_config_value.md) — запись в config
- [x] [get_project_paths](docs/16-project-settings/get_project_paths.md) — пути проекта (Content, Config, Plugins)

### 17. Source Control
- [x] [scc_checkout](docs/17-source-control/scc_checkout.md) — checkout файлов
- [x] [scc_add](docs/17-source-control/scc_add.md) — добавление файлов
- [x] [scc_delete](docs/17-source-control/scc_delete.md) — удаление
- [x] [scc_revert](docs/17-source-control/scc_revert.md) — откат
- [x] [scc_status](docs/17-source-control/scc_status.md) — статус файлов
- [x] [scc_submit](docs/17-source-control/scc_submit.md) — коммит изменений
- [x] [scc_sync](docs/17-source-control/scc_sync.md) — синхронизация

### 18. UI и нотификации
- [x] [show_notification](docs/18-ui-notifications/show_notification.md) — toast-уведомление в редакторе
- [x] [show_dialog](docs/18-ui-notifications/show_dialog.md) — модальный диалог
- [x] [open_asset_editor](docs/18-ui-notifications/open_asset_editor.md) — открытие редактора ассета
- [x] [close_asset_editor](docs/18-ui-notifications/close_asset_editor.md) — закрытие
- [x] [sync_content_browser](docs/18-ui-notifications/sync_content_browser.md) — навигация в Content Browser
- [x] [get_selected_content_browser](docs/18-ui-notifications/get_selected_content_browser.md) — текущее выделение в CB

### 19. World Partition
- [x] [build_hlod_wp](docs/19-world-partition/build_hlod_wp.md) — сборка HLOD для World Partition
- [x] [delete_hlod_wp](docs/19-world-partition/delete_hlod_wp.md) — удаление HLOD

### 20. Play In Editor (PIE)
- [x] [play_in_editor](docs/20-play-in-editor/play_in_editor.md) — запуск PIE
- [x] [stop_pie](docs/20-play-in-editor/stop_pie.md) — остановка
- [x] [simulate_in_editor](docs/20-play-in-editor/simulate_in_editor.md) — режим Simulate
- [x] [is_playing](docs/20-play-in-editor/is_playing.md) — проверка статуса PIE

### 21. Batch-операции
- [x] [batch_set_actor_folders](docs/21-batch-operations/batch_set_actor_folders.md) — массовая установка папок акторов в Outliner (до 100 за вызов)
- [x] [batch_set_actor_properties](docs/21-batch-operations/batch_set_actor_properties.md) — массовая установка свойств акторов (до 100 за вызов)
- [x] [batch_set_material_expression_properties](docs/21-batch-operations/batch_set_material_expression_properties.md) — массовая установка свойств нод материала (до 100 за вызов)
- [x] [batch_connect_material_expressions](docs/21-batch-operations/batch_connect_material_expressions.md) — массовое соединение нод материала (до 100 за вызов)
- [x] [batch_set_actor_transforms](docs/21-batch-operations/batch_set_actor_transforms.md) — массовая установка трансформов акторов (до 100 за вызов)

### 22. Blueprint Graph Nodes
- [x] [get_graph_nodes](docs/22-blueprint-graph-nodes/get_graph_nodes.md) — чтение всех нод в графе (node_id, class, title, position, size, pins, comment)
- [x] [set_node_position](docs/22-blueprint-graph-nodes/set_node_position.md) — перемещение ноды на графе
- [x] [batch_set_node_positions](docs/22-blueprint-graph-nodes/batch_set_node_positions.md) — массовое перемещение нод (до 100 за вызов)
- [x] [add_comment_box](docs/22-blueprint-graph-nodes/add_comment_box.md) — добавление Comment Box (EdGraphNode_Comment)
- [x] [delete_comment_box](docs/22-blueprint-graph-nodes/delete_comment_box.md) — удаление Comment Box
- [x] [set_comment_box_properties](docs/22-blueprint-graph-nodes/set_comment_box_properties.md) — изменение свойств Comment Box (текст, цвет, позиция, размер)
- [x] [get_graph_nodes_summary](docs/22-blueprint-graph-nodes/get_graph_nodes_summary.md) — облегчённый список нод графа (id, class, title, connected_node_ids) с опциональным class_filter
- [x] [get_graph_nodes_in_area](docs/22-blueprint-graph-nodes/get_graph_nodes_in_area.md) — пространственный запрос нод в прямоугольной области (collision detection для размещения)

### 23. Blueprint Components
- [x] [add_blueprint_component](docs/23-blueprint-components/add_blueprint_component.md) — добавление компонента в блупринт (класс, имя, parent)
- [x] [remove_blueprint_component](docs/23-blueprint-components/remove_blueprint_component.md) — удаление компонента из блупринта
- [x] [get_blueprint_components](docs/23-blueprint-components/get_blueprint_components.md) — список всех компонентов блупринта (name, class, parent, is_root)
- [x] [set_blueprint_component_property](docs/23-blueprint-components/set_blueprint_component_property.md) — установка свойства компонента (default value)
- [x] [get_blueprint_component_property](docs/23-blueprint-components/get_blueprint_component_property.md) — чтение свойства компонента

### 24. Blueprint Graph Editing
- [x] [add_graph_node](docs/24-blueprint-graph-editing/add_graph_node.md) — добавление ноды в граф (CallFunction, Event, ComponentBoundEvent, VariableGet, VariableSet, DynamicCast, IfThenElse/Branch, MacroInstance, SwitchEnum)
- [x] [connect_graph_pins](docs/24-blueprint-graph-editing/connect_graph_pins.md) — соединение пинов между нодами
- [x] [disconnect_graph_pins](docs/24-blueprint-graph-editing/disconnect_graph_pins.md) — разъединение соединённых пинов
- [x] [set_pin_default_value](docs/24-blueprint-graph-editing/set_pin_default_value.md) — установка значения по умолчанию на пине
- [x] [delete_graph_node](docs/24-blueprint-graph-editing/delete_graph_node.md) — удаление ноды из графа
- [x] [add_graph_nodes_batch](docs/24-blueprint-graph-editing/add_graph_nodes_batch.md) — массовое создание нод + соединений за один вызов

### 25. UMG Widgets
- [ ] [create_widget_blueprint](docs/25-umg-widgets/create_widget_blueprint.md) — создание Widget Blueprint с корневым виджетом
- [ ] [get_widget_tree](docs/25-umg-widgets/get_widget_tree.md) — получение дерева виджетов
- [ ] [add_widget](docs/25-umg-widgets/add_widget.md) — добавление виджета в панель
- [ ] [remove_widget](docs/25-umg-widgets/remove_widget.md) — удаление виджета из дерева
- [ ] [move_widget](docs/25-umg-widgets/move_widget.md) — перемещение виджета в другой контейнер
- [ ] [rename_widget](docs/25-umg-widgets/rename_widget.md) — переименование виджета
- [ ] [replace_widget](docs/25-umg-widgets/replace_widget.md) — замена типа виджета с сохранением детей
- [ ] [set_widget_property](docs/25-umg-widgets/set_widget_property.md) — установка свойства виджета (Text, Color, Visibility и т.д.)
- [ ] [get_widget_property](docs/25-umg-widgets/get_widget_property.md) — чтение свойства виджета
- [ ] [set_widget_slot](docs/25-umg-widgets/set_widget_slot.md) — настройка слота (anchors, padding, alignment, size)
- [ ] [get_widget_animations](docs/25-umg-widgets/get_widget_animations.md) — список анимаций Widget Blueprint
- [ ] [export_widgets](docs/25-umg-widgets/export_widgets.md) — экспорт виджетов в текст
- [ ] [import_widgets](docs/25-umg-widgets/import_widgets.md) — импорт виджетов из текста
