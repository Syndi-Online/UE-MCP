# UE-MCP

MCP (Model Context Protocol) Server plugin for Unreal Engine 5.7. Runs an HTTP server inside the editor, exposing engine functionality via JSON-RPC 2.0.

## Status

- **Transport:** HTTP on `localhost:8080/mcp`
- **Protocol:** MCP 2025-03-26
- [x] Plugin skeleton + HTTP server
- [x] MCP protocol handler (initialize, tools/list, tools/call, ping)
- [x] Session management (Mcp-Session-Id)
- [x] hello_world test tool

## Tools Checklist

### 1. Управление акторами
- [x] spawn_actor — спавн из класса или ассета
- [x] delete_actor — удаление акторов
- [x] duplicate_actor — дублирование
- [x] get_actors_in_level — список всех акторов на уровне
- [x] select_actor — выделение акторов
- [x] deselect_all — снятие выделения
- [x] get_actor_transform — чтение трансформа
- [x] set_actor_transform — установка трансформа
- [x] get_actor_property — чтение свойств актора
- [x] set_actor_property — установка свойств актора
- [x] convert_actor — конвертация типа актора
- [x] move_actors_to_level — перемещение между уровнями
- [x] group_actors — группировка акторов
- [x] set_actor_folder — управление папками в Outliner

### 2. Управление уровнями
- [x] new_level — создание нового уровня
- [x] new_level_from_template — создание из шаблона
- [x] load_level — загрузка уровня
- [x] save_level — сохранение текущего уровня
- [x] save_all_dirty_levels — сохранение всех изменённых
- [x] add_sublevel — добавление сабуровня (streaming)
- [x] remove_sublevel — удаление сабуровня
- [x] set_current_level — переключение текущего уровня
- [x] set_level_visibility — управление видимостью уровня

### 3. Работа с ассетами
- [x] load_asset — загрузка ассета по пути
- [x] create_asset — создание нового ассета
- [x] duplicate_asset — дублирование
- [x] rename_asset — переименование
- [x] delete_asset — удаление
- [x] save_asset — сохранение
- [x] find_assets — поиск по пути, классу, тегам
- [x] list_assets — список ассетов в директории
- [x] import_asset — импорт файлов (FBX, текстуры, аудио)
- [x] export_asset — экспорт
- [x] get_asset_dependencies — зависимости ассета
- [x] get_asset_referencers — кто ссылается на ассет
- [x] get_asset_metadata — чтение метаданных
- [x] set_asset_metadata — установка метаданных

### 4. Вьюпорт и камера
- [x] get_viewport_camera — чтение позиции/ротации камеры
- [x] set_viewport_camera — установка позиции/ротации
- [x] set_game_view — переключение Game View
- [x] set_viewport_realtime — переключение Realtime
- [x] set_view_mode — режим (Lit/Wireframe/Unlit)
- [x] set_camera_fov — изменение FOV
- [x] focus_on_actors — фокус на акторах
- [x] take_screenshot — скриншот вьюпорта
- [x] pilot_actor — привязка камеры к актору
- [x] eject_pilot — отвязка камеры
- [x] set_bookmark — сохранение закладки камеры
- [x] jump_to_bookmark — переход к закладке

### 5. Blueprints
- [x] create_blueprint — создание Blueprint из класса
- [x] create_blueprint_from_actor — создание из актора
- [x] compile_blueprint — компиляция
- [x] add_blueprint_variable — добавление переменной
- [x] remove_blueprint_variable — удаление переменной
- [x] add_blueprint_function — добавление функции
- [x] add_blueprint_interface — добавление интерфейса
- [x] get_blueprint_graphs — список графов
- [x] reparent_blueprint — смена родительского класса
- [x] open_blueprint_editor — открытие в редакторе

### 6. Материалы
- [x] create_material_expression — создание ноды
- [x] delete_material_expression — удаление ноды
- [x] connect_material_expressions — соединение нод
- [x] connect_material_property — подключение к выходу
- [x] recompile_material — рекомпиляция
- [x] get_material_parameter — чтение параметра (scalar/vector/texture)
- [x] set_material_instance_parameter — установка параметра MI
- [x] set_material_instance_parent — смена родителя MI
- [x] get_material_statistics — статистика шейдера

### 7. Static Mesh
- [x] set_static_mesh_lod — настройка LOD
- [x] import_lod — импорт LOD из FBX
- [x] get_lod_settings — чтение настроек LOD
- [x] set_collision — настройка коллизий
- [x] add_uv_channel — добавление UV канала
- [x] remove_uv_channel — удаление UV канала
- [x] generate_uv_channel — генерация UV (planar/cylindrical/box)
- [x] set_mesh_material — назначение материала по слоту
- [x] get_mesh_bounds — bounding box

### 8. Skeletal Mesh и анимация
- [x] regenerate_skeletal_lod — регенерация LOD
- [x] get_skeleton_info — bone count, vertex count
- [x] get_physics_asset — чтение Physics Asset
- [x] set_physics_asset — установка Physics Asset
- [x] reimport_skeletal_mesh — реимпорт
- [x] create_anim_asset — создание AnimSequence/BlendSpace/Montage
- [x] create_anim_blueprint — создание Anim Blueprint
- [x] apply_anim_compression — применение компрессии

### 9. Секвенсер / Кинематика
- [x] sequencer_play — воспроизведение
- [x] sequencer_pause — пауза
- [x] sequencer_stop — остановка
- [x] sequencer_set_time — установка позиции на таймлайне
- [x] sequencer_get_time — чтение позиции
- [x] sequencer_set_playback_range — диапазон воспроизведения
- [x] sequencer_add_actor — добавление актора
- [x] sequencer_add_track — добавление трека
- [x] sequencer_add_marker — добавление маркера
- [x] sequencer_force_evaluate — принудительная оценка

### 10. Ландшафт
- [x] get_landscape_heightmap — чтение heightmap
- [x] set_landscape_heightmap — запись heightmap
- [x] get_landscape_weightmap — чтение weight map
- [x] set_landscape_weightmap — запись weight map
- [x] import_landscape — импорт
- [x] export_landscape — экспорт
- [x] rebuild_landscape — перестройка данных

### 11. Фолиаж
- [x] add_foliage_instances — добавление инстансов
- [x] remove_foliage_instances — удаление инстансов
- [x] register_foliage_mesh — регистрация меша
- [x] resimulate_procedural_foliage — процедурная генерация

### 12. Навигация / AI
- [x] build_navigation — построение NavMesh
- [x] rebuild_navigation — перестройка
- [x] find_path — поиск пути между точками
- [x] project_point_to_nav — проецирование на NavMesh
- [x] get_random_reachable_point — случайная достижимая позиция

### 13. Физика
- [x] create_physics_collision — настройка коллизий BodySetup
- [x] set_physical_material — настройка физического материала
- [x] set_physics_velocity — установка скорости (linear/angular)
- [x] manage_constraints — управление constraint'ами

### 14. Сборка (Build)
- [x] build_geometry — сборка геометрии
- [x] build_lighting — сборка освещения
- [x] build_navigation_data — сборка AI Paths
- [x] build_hlod — сборка HLOD
- [x] build_texture_streaming — сборка Texture Streaming
- [x] build_all — полная сборка
- [x] get_build_status — статус текущей сборки

### 15. Консольные команды
- [x] execute_console_command — выполнение консольной команды
- [x] get_cvar — чтение console variable
- [x] set_cvar — установка console variable

### 16. Настройки проекта
- [x] get_project_info — информация о проекте
- [x] get_config_value — чтение из config
- [x] set_config_value — запись в config
- [x] get_project_paths — пути проекта (Content, Config, Plugins)

### 17. Source Control
- [ ] scc_checkout — checkout файлов
- [ ] scc_add — добавление файлов
- [ ] scc_delete — удаление
- [ ] scc_revert — откат
- [ ] scc_status — статус файлов
- [ ] scc_submit — коммит изменений
- [ ] scc_sync — синхронизация

### 18. UI и нотификации
- [ ] show_notification — toast-уведомление в редакторе
- [ ] show_dialog — модальный диалог
- [ ] open_asset_editor — открытие редактора ассета
- [ ] close_asset_editor — закрытие
- [ ] sync_content_browser — навигация в Content Browser
- [ ] get_selected_content_browser — текущее выделение в CB

### 19. World Partition
- [ ] build_hlod_wp — сборка HLOD для World Partition
- [ ] delete_hlod_wp — удаление HLOD

### 20. Play In Editor (PIE)
- [ ] play_in_editor — запуск PIE
- [ ] stop_pie — остановка
- [ ] simulate_in_editor — режим Simulate
- [ ] is_playing — проверка статуса PIE
