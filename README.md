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
- [ ] spawn_actor — спавн из класса или ассета
- [ ] delete_actor — удаление акторов
- [ ] duplicate_actor — дублирование
- [ ] get_actors_in_level — список всех акторов на уровне
- [ ] select_actor — выделение акторов
- [ ] deselect_all — снятие выделения
- [ ] get_actor_transform — чтение трансформа
- [ ] set_actor_transform — установка трансформа
- [ ] get_actor_property — чтение свойств актора
- [ ] set_actor_property — установка свойств актора
- [ ] convert_actor — конвертация типа актора
- [ ] move_actors_to_level — перемещение между уровнями
- [ ] group_actors — группировка акторов
- [ ] set_actor_folder — управление папками в Outliner

### 2. Управление уровнями
- [ ] new_level — создание нового уровня
- [ ] new_level_from_template — создание из шаблона
- [ ] load_level — загрузка уровня
- [ ] save_level — сохранение текущего уровня
- [ ] save_all_dirty_levels — сохранение всех изменённых
- [ ] add_sublevel — добавление сабуровня (streaming)
- [ ] remove_sublevel — удаление сабуровня
- [ ] set_current_level — переключение текущего уровня
- [ ] set_level_visibility — управление видимостью уровня

### 3. Работа с ассетами
- [ ] load_asset — загрузка ассета по пути
- [ ] create_asset — создание нового ассета
- [ ] duplicate_asset — дублирование
- [ ] rename_asset — переименование
- [ ] delete_asset — удаление
- [ ] save_asset — сохранение
- [ ] find_assets — поиск по пути, классу, тегам
- [ ] list_assets — список ассетов в директории
- [ ] import_asset — импорт файлов (FBX, текстуры, аудио)
- [ ] export_asset — экспорт
- [ ] get_asset_dependencies — зависимости ассета
- [ ] get_asset_referencers — кто ссылается на ассет
- [ ] get_asset_metadata — чтение метаданных
- [ ] set_asset_metadata — установка метаданных

### 4. Вьюпорт и камера
- [ ] get_viewport_camera — чтение позиции/ротации камеры
- [ ] set_viewport_camera — установка позиции/ротации
- [ ] set_game_view — переключение Game View
- [ ] set_viewport_realtime — переключение Realtime
- [ ] set_view_mode — режим (Lit/Wireframe/Unlit)
- [ ] set_camera_fov — изменение FOV
- [ ] focus_on_actors — фокус на акторах
- [ ] take_screenshot — скриншот вьюпорта
- [ ] pilot_actor — привязка камеры к актору
- [ ] eject_pilot — отвязка камеры
- [ ] set_bookmark — сохранение закладки камеры
- [ ] jump_to_bookmark — переход к закладке

### 5. Blueprints
- [ ] create_blueprint — создание Blueprint из класса
- [ ] create_blueprint_from_actor — создание из актора
- [ ] compile_blueprint — компиляция
- [ ] add_blueprint_variable — добавление переменной
- [ ] remove_blueprint_variable — удаление переменной
- [ ] add_blueprint_function — добавление функции
- [ ] add_blueprint_interface — добавление интерфейса
- [ ] get_blueprint_graphs — список графов
- [ ] reparent_blueprint — смена родительского класса
- [ ] open_blueprint_editor — открытие в редакторе

### 6. Материалы
- [ ] create_material_expression — создание ноды
- [ ] delete_material_expression — удаление ноды
- [ ] connect_material_expressions — соединение нод
- [ ] connect_material_property — подключение к выходу
- [ ] recompile_material — рекомпиляция
- [ ] get_material_parameter — чтение параметра (scalar/vector/texture)
- [ ] set_material_instance_parameter — установка параметра MI
- [ ] set_material_instance_parent — смена родителя MI
- [ ] get_material_statistics — статистика шейдера

### 7. Static Mesh
- [ ] set_static_mesh_lod — настройка LOD
- [ ] import_lod — импорт LOD из FBX
- [ ] get_lod_settings — чтение настроек LOD
- [ ] set_collision — настройка коллизий
- [ ] add_uv_channel — добавление UV канала
- [ ] remove_uv_channel — удаление UV канала
- [ ] generate_uv_channel — генерация UV (planar/cylindrical/box)
- [ ] set_mesh_material — назначение материала по слоту
- [ ] get_mesh_bounds — bounding box

### 8. Skeletal Mesh и анимация
- [ ] regenerate_skeletal_lod — регенерация LOD
- [ ] get_skeleton_info — bone count, vertex count
- [ ] get_physics_asset — чтение Physics Asset
- [ ] set_physics_asset — установка Physics Asset
- [ ] reimport_skeletal_mesh — реимпорт
- [ ] create_anim_asset — создание AnimSequence/BlendSpace/Montage
- [ ] create_anim_blueprint — создание Anim Blueprint
- [ ] apply_anim_compression — применение компрессии

### 9. Секвенсер / Кинематика
- [ ] sequencer_play — воспроизведение
- [ ] sequencer_pause — пауза
- [ ] sequencer_stop — остановка
- [ ] sequencer_set_time — установка позиции на таймлайне
- [ ] sequencer_get_time — чтение позиции
- [ ] sequencer_set_playback_range — диапазон воспроизведения
- [ ] sequencer_add_actor — добавление актора
- [ ] sequencer_add_track — добавление трека
- [ ] sequencer_add_marker — добавление маркера
- [ ] sequencer_force_evaluate — принудительная оценка

### 10. Ландшафт
- [ ] get_landscape_heightmap — чтение heightmap
- [ ] set_landscape_heightmap — запись heightmap
- [ ] get_landscape_weightmap — чтение weight map
- [ ] set_landscape_weightmap — запись weight map
- [ ] import_landscape — импорт
- [ ] export_landscape — экспорт
- [ ] rebuild_landscape — перестройка данных

### 11. Фолиаж
- [ ] add_foliage_instances — добавление инстансов
- [ ] remove_foliage_instances — удаление инстансов
- [ ] register_foliage_mesh — регистрация меша
- [ ] resimulate_procedural_foliage — процедурная генерация

### 12. Навигация / AI
- [ ] build_navigation — построение NavMesh
- [ ] rebuild_navigation — перестройка
- [ ] find_path — поиск пути между точками
- [ ] project_point_to_nav — проецирование на NavMesh
- [ ] get_random_reachable_point — случайная достижимая позиция

### 13. Физика
- [ ] create_physics_collision — настройка коллизий BodySetup
- [ ] set_physical_material — настройка физического материала
- [ ] set_physics_velocity — установка скорости (linear/angular)
- [ ] manage_constraints — управление constraint'ами

### 14. Сборка (Build)
- [ ] build_geometry — сборка геометрии
- [ ] build_lighting — сборка освещения
- [ ] build_navigation_data — сборка AI Paths
- [ ] build_hlod — сборка HLOD
- [ ] build_texture_streaming — сборка Texture Streaming
- [ ] build_all — полная сборка
- [ ] get_build_status — статус текущей сборки

### 15. Консольные команды
- [ ] execute_console_command — выполнение консольной команды
- [ ] get_cvar — чтение console variable
- [ ] set_cvar — установка console variable

### 16. Настройки проекта
- [ ] get_project_info — информация о проекте
- [ ] get_config_value — чтение из config
- [ ] set_config_value — запись в config
- [ ] get_project_paths — пути проекта (Content, Config, Plugins)

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
