// Import declarations for core library, generated by DylibTool
#ifndef core_IMPORTS_H
#define core_IMPORTS_H

#include "Win32Defs.h"
#include <stdint.h>

// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define tobii_sdk_error_destroy tobii_sdk_error_destroy_4e1f__
#define tobii_sdk_error_clone tobii_sdk_error_clone_6e1b__
#define tobii_sdk_error_get_message tobii_sdk_error_get_message_297e__
#define tobii_sdk_error_get_code tobii_sdk_error_get_code_7b39__
#define tobii_sdk_error_to_string tobii_sdk_error_to_string_7135__
#define tobii_sdk_error_is_failure tobii_sdk_error_is_failure_2348__
#define tobii_sdk_error_convert_code_to_string tobii_sdk_error_convert_code_to_string_4f39__
#define tobii_sdk_error_create_generic tobii_sdk_error_create_generic_6a26__
#define tobii_sdk_blob_create tobii_sdk_blob_create_3b4b__
#define tobii_sdk_blob_get_size tobii_sdk_blob_get_size_702e__
#define tobii_sdk_blob_get_data tobii_sdk_blob_get_data_4f17__
#define tobii_sdk_blob_destroy tobii_sdk_blob_destroy_5379__
#define tobii_sdk_mainloop_create tobii_sdk_mainloop_create_7104__
#define tobii_sdk_mainloop_run tobii_sdk_mainloop_run_7e42__
#define tobii_sdk_mainloop_quit tobii_sdk_mainloop_quit_125e__
#define tobii_sdk_mainloop_destroy tobii_sdk_mainloop_destroy_5d68__
#define tobii_sdk_device_info_iterator_move_next tobii_sdk_device_info_iterator_move_next_7e44__
#define tobii_sdk_device_info_iterator_get_key_name tobii_sdk_device_info_iterator_get_key_name_2608__
#define tobii_sdk_device_info_iterator_destroy tobii_sdk_device_info_iterator_destroy_5c77__
#define tobii_sdk_device_info_clone tobii_sdk_device_info_clone_2c5b__
#define tobii_sdk_device_info_create_test tobii_sdk_device_info_create_test_3b07__
#define tobii_sdk_device_info_get_product_id tobii_sdk_device_info_get_product_id_3a49__
#define tobii_sdk_device_info_get tobii_sdk_device_info_get_221b__
#define tobii_sdk_device_info_get_iterator tobii_sdk_device_info_get_iterator_210e__
#define tobii_sdk_device_info_get_factory_info tobii_sdk_device_info_get_factory_info_0e37__
#define tobii_sdk_device_info_destroy tobii_sdk_device_info_destroy_3555__
#define tobii_sdk_factory_info_create_for_networked_eyetracker tobii_sdk_factory_info_create_for_networked_eyetracker_1e52__
#define tobii_sdk_factory_info_get_representation tobii_sdk_factory_info_get_representation_7c5b__
#define tobii_sdk_factory_info_destroy tobii_sdk_factory_info_destroy_424e__
#define tobii_sdk_device_browser_create tobii_sdk_device_browser_create_1473__
#define tobii_sdk_device_browser_destroy tobii_sdk_device_browser_destroy_6f2a__
#define tobii_sdk_device_browser_get_device_list tobii_sdk_device_browser_get_device_list_4e03__
#define tobii_sdk_device_info_list_get_size tobii_sdk_device_info_list_get_size_1800__
#define tobii_sdk_device_info_list_get_at_index tobii_sdk_device_info_list_get_at_index_5d2c__
#define tobii_sdk_device_info_list_destroy tobii_sdk_device_info_list_destroy_503c__
#define tobii_sdk_vector_type_is_compatible tobii_sdk_vector_type_is_compatible_1643__
#define tobii_sdk_vector_create tobii_sdk_vector_create_7e1d__
#define tobii_sdk_vector_destroy tobii_sdk_vector_destroy_2414__
#define tobii_sdk_vector_get_element_type tobii_sdk_vector_get_element_type_1502__
#define tobii_sdk_vector_get_length tobii_sdk_vector_get_length_5d45__
#define tobii_sdk_vector_get_int32 tobii_sdk_vector_get_int32_5825__
#define tobii_sdk_vector_get_uint32 tobii_sdk_vector_get_uint32_166b__
#define tobii_sdk_vector_get_fixed_15x16_as_float32 tobii_sdk_vector_get_fixed_15x16_as_float32_5b4c__
#define tobii_sdk_vector_get_fixed_22x41_as_float64 tobii_sdk_vector_get_fixed_22x41_as_float64_3a4d__
#define tobii_sdk_vector_get_string tobii_sdk_vector_get_string_3e0a__
#define tobii_sdk_vector_get_string_length tobii_sdk_vector_get_string_length_2509__
#define tobii_sdk_vector_set_int32 tobii_sdk_vector_set_int32_522d__
#define tobii_sdk_vector_set_uint32 tobii_sdk_vector_set_uint32_7e01__
#define tobii_sdk_vector_set_fixed_15x16_as_float32 tobii_sdk_vector_set_fixed_15x16_as_float32_4752__
#define tobii_sdk_vector_set_fixed_22x41_as_float64 tobii_sdk_vector_set_fixed_22x41_as_float64_5227__
#define tobii_sdk_vector_set_string tobii_sdk_vector_set_string_4115__
#define tobii_sdk_param_stack_create tobii_sdk_param_stack_create_1762__
#define tobii_sdk_param_stack_create_test_stack tobii_sdk_param_stack_create_test_stack_5558__
#define tobii_sdk_param_stack_clone tobii_sdk_param_stack_clone_5513__
#define tobii_sdk_param_stack_destroy tobii_sdk_param_stack_destroy_6224__
#define tobii_sdk_param_stack_get_size tobii_sdk_param_stack_get_size_7d0a__
#define tobii_sdk_param_stack_get_type tobii_sdk_param_stack_get_type_241a__
#define tobii_sdk_param_stack_append tobii_sdk_param_stack_append_7f5b__
#define tobii_sdk_param_stack_get_int32 tobii_sdk_param_stack_get_int32_5f27__
#define tobii_sdk_param_stack_get_uint32 tobii_sdk_param_stack_get_uint32_265e__
#define tobii_sdk_param_stack_get_int64 tobii_sdk_param_stack_get_int64_5f27__
#define tobii_sdk_param_stack_get_uint64 tobii_sdk_param_stack_get_uint64_255d__
#define tobii_sdk_param_stack_get_float32 tobii_sdk_param_stack_get_float32_194b__
#define tobii_sdk_param_stack_get_float64 tobii_sdk_param_stack_get_float64_1f2b__
#define tobii_sdk_param_stack_get_fixed_15x16_as_float32 tobii_sdk_param_stack_get_fixed_15x16_as_float32_5d4f__
#define tobii_sdk_param_stack_get_fixed_22x41_as_float64 tobii_sdk_param_stack_get_fixed_22x41_as_float64_394b__
#define tobii_sdk_param_stack_get_node_prolog tobii_sdk_param_stack_get_node_prolog_7059__
#define tobii_sdk_param_stack_get_string tobii_sdk_param_stack_get_string_1c2d__
#define tobii_sdk_param_stack_get_string_length tobii_sdk_param_stack_get_string_length_062f__
#define tobii_sdk_param_stack_get_blob_length tobii_sdk_param_stack_get_blob_length_645b__
#define tobii_sdk_param_stack_get_vector tobii_sdk_param_stack_get_vector_5a0d__
#define tobii_sdk_param_stack_push_int32 tobii_sdk_param_stack_push_int32_126a__
#define tobii_sdk_param_stack_push_uint32 tobii_sdk_param_stack_push_uint32_770f__
#define tobii_sdk_param_stack_push_int64 tobii_sdk_param_stack_push_int64_126a__
#define tobii_sdk_param_stack_push_uint64 tobii_sdk_param_stack_push_uint64_740c__
#define tobii_sdk_param_stack_push_string tobii_sdk_param_stack_push_string_7e16__
#define tobii_sdk_param_stack_push_float32_as_fixed_15x16 tobii_sdk_param_stack_push_float32_as_fixed_15x16_3123__
#define tobii_sdk_param_stack_push_float64_as_fixed_22x41 tobii_sdk_param_stack_push_float64_as_fixed_22x41_4436__
#define tobii_sdk_param_stack_push_node_prolog tobii_sdk_param_stack_push_node_prolog_1831__
#define tobii_sdk_param_stack_push_float32 tobii_sdk_param_stack_push_float32_7e2c__
#define tobii_sdk_param_stack_push_float64 tobii_sdk_param_stack_push_float64_6a5e__
#define tobii_sdk_param_stack_push_blob tobii_sdk_param_stack_push_blob_4d72__
#define tobii_sdk_param_stack_push_vector tobii_sdk_param_stack_push_vector_1373__
#define tobii_sdk_callback_connection_destroy tobii_sdk_callback_connection_destroy_274e__
#define tobii_sdk_message_passer_get tobii_sdk_message_passer_get_7774__
#define tobii_sdk_message_passer_destroy tobii_sdk_message_passer_destroy_673a__
#define tobii_sdk_message_passer_enable_nop_sending tobii_sdk_message_passer_enable_nop_sending_0814__
#define tobii_sdk_message_passer_disable_nop_sending tobii_sdk_message_passer_disable_nop_sending_5713__
#define tobii_sdk_message_passer_execute_request tobii_sdk_message_passer_execute_request_4823__
#define tobii_sdk_message_passer_add_data_handler tobii_sdk_message_passer_add_data_handler_634b__
#define tobii_sdk_message_passer_add_error_handler tobii_sdk_message_passer_add_error_handler_296b__
#define tobii_sdk_clock_get_default tobii_sdk_clock_get_default_3605__
#define tobii_sdk_clock_create_user_defined tobii_sdk_clock_create_user_defined_3808__
#define tobii_sdk_clock_get_time tobii_sdk_clock_get_time_3162__
#define tobii_sdk_clock_get_resolution tobii_sdk_clock_get_resolution_5e16__
#define tobii_sdk_clock_destroy tobii_sdk_clock_destroy_5c1d__
#define tobii_sdk_sync_state_clone tobii_sdk_sync_state_clone_4b64__
#define tobii_sdk_sync_state_destroy tobii_sdk_sync_state_destroy_144a__
#define tobii_sdk_sync_state_get_sync_state_flag tobii_sdk_sync_state_get_sync_state_flag_355d__
#define tobii_sdk_sync_state_get_number_of_points_in_use tobii_sdk_sync_state_get_number_of_points_in_use_312a__
#define tobii_sdk_sync_state_get_point_in_use tobii_sdk_sync_state_get_point_in_use_724d__
#define tobii_sdk_sync_state_get_error_approximation tobii_sdk_sync_state_get_error_approximation_4107__
#define tobii_sdk_sync_manager_create tobii_sdk_sync_manager_create_3d20__
#define tobii_sdk_sync_manager_convert_from_local_to_remote tobii_sdk_sync_manager_convert_from_local_to_remote_7819__
#define tobii_sdk_sync_manager_convert_from_remote_to_local tobii_sdk_sync_manager_convert_from_remote_to_local_777f__
#define tobii_sdk_sync_manager_get_sync_state tobii_sdk_sync_manager_get_sync_state_2739__
#define tobii_sdk_sync_manager_destroy tobii_sdk_sync_manager_destroy_797b__
#define tobii_sdk_upgrade_package_create tobii_sdk_upgrade_package_create_1762__
#define tobii_sdk_upgrade_package_parse tobii_sdk_upgrade_package_parse_0c70__
#define tobii_sdk_upgrade_package_destroy tobii_sdk_upgrade_package_destroy_1142__
#define tobii_sdk_upgrade_package_get_num_parts tobii_sdk_upgrade_package_get_num_parts_2a00__
#define tobii_sdk_upgrade_package_get_part tobii_sdk_upgrade_package_get_part_740a__
#define tobii_sdk_upgrade_package_append_part tobii_sdk_upgrade_package_append_part_0f58__
#define tobii_sdk_upgrade_package_finalize tobii_sdk_upgrade_package_finalize_723f__
#define tobii_sdk_upgrade_package_finalized_get_data tobii_sdk_upgrade_package_finalized_get_data_391c__
#define tobii_sdk_upgrade_package_finalized_get_size tobii_sdk_upgrade_package_finalized_get_size_791f__
#define tobii_sdk_upgrade_package_finalized_destroy tobii_sdk_upgrade_package_finalized_destroy_5d0a__
#define tobii_sdk_upgrade_package_part_create_raw_part tobii_sdk_upgrade_package_part_create_raw_part_0936__
#define tobii_sdk_upgrade_package_part_create_legacy_metadata_part tobii_sdk_upgrade_package_part_create_legacy_metadata_part_6247__
#define tobii_sdk_upgrade_package_part_create_legacy_file_part tobii_sdk_upgrade_package_part_create_legacy_file_part_0566__
#define tobii_sdk_upgrade_package_part_create_legacy_exec_command_part tobii_sdk_upgrade_package_part_create_legacy_exec_command_part_5e1f__
#define tobii_sdk_upgrade_package_part_create_cancancel_part tobii_sdk_upgrade_package_part_create_cancancel_part_6b15__
#define tobii_sdk_upgrade_package_part_get_representation tobii_sdk_upgrade_package_part_get_representation_5404__
#define tobii_sdk_upgrade_package_part_destroy tobii_sdk_upgrade_package_part_destroy_5c62__
#define tobii_sdk_upgrade_progress_reporter_create tobii_sdk_upgrade_progress_reporter_create_3f7f__
#define tobii_sdk_upgrade_progress_reporter_get_progress tobii_sdk_upgrade_progress_reporter_get_progress_724a__
#define tobii_sdk_upgrade_progress_get_can_cancel tobii_sdk_upgrade_progress_get_can_cancel_6a1a__
#define tobii_sdk_upgrade_progress_cancel tobii_sdk_upgrade_progress_cancel_6e04__
#define tobii_sdk_upgrade_progress_reporter_destroy tobii_sdk_upgrade_progress_reporter_destroy_6a18__
#define tobii_sdk_upgrade_package_is_compatible_with_device tobii_sdk_upgrade_package_is_compatible_with_device_292a__
#define tobii_sdk_upgrade_begin tobii_sdk_upgrade_begin_0027__
#define tobii_sdk_init tobii_sdk_init_694d__
#define tobii_sdk_free_string tobii_sdk_free_string_317d__
#define tobii_sdk_hmac_perform tobii_sdk_hmac_perform_375c__

#include "core.h"

// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef tobii_sdk_error_destroy
#undef tobii_sdk_error_clone
#undef tobii_sdk_error_get_message
#undef tobii_sdk_error_get_code
#undef tobii_sdk_error_to_string
#undef tobii_sdk_error_is_failure
#undef tobii_sdk_error_convert_code_to_string
#undef tobii_sdk_error_create_generic
#undef tobii_sdk_blob_create
#undef tobii_sdk_blob_get_size
#undef tobii_sdk_blob_get_data
#undef tobii_sdk_blob_destroy
#undef tobii_sdk_mainloop_create
#undef tobii_sdk_mainloop_run
#undef tobii_sdk_mainloop_quit
#undef tobii_sdk_mainloop_destroy
#undef tobii_sdk_device_info_iterator_move_next
#undef tobii_sdk_device_info_iterator_get_key_name
#undef tobii_sdk_device_info_iterator_destroy
#undef tobii_sdk_device_info_clone
#undef tobii_sdk_device_info_create_test
#undef tobii_sdk_device_info_get_product_id
#undef tobii_sdk_device_info_get
#undef tobii_sdk_device_info_get_iterator
#undef tobii_sdk_device_info_get_factory_info
#undef tobii_sdk_device_info_destroy
#undef tobii_sdk_factory_info_create_for_networked_eyetracker
#undef tobii_sdk_factory_info_get_representation
#undef tobii_sdk_factory_info_destroy
#undef tobii_sdk_device_browser_create
#undef tobii_sdk_device_browser_destroy
#undef tobii_sdk_device_browser_get_device_list
#undef tobii_sdk_device_info_list_get_size
#undef tobii_sdk_device_info_list_get_at_index
#undef tobii_sdk_device_info_list_destroy
#undef tobii_sdk_vector_type_is_compatible
#undef tobii_sdk_vector_create
#undef tobii_sdk_vector_destroy
#undef tobii_sdk_vector_get_element_type
#undef tobii_sdk_vector_get_length
#undef tobii_sdk_vector_get_int32
#undef tobii_sdk_vector_get_uint32
#undef tobii_sdk_vector_get_fixed_15x16_as_float32
#undef tobii_sdk_vector_get_fixed_22x41_as_float64
#undef tobii_sdk_vector_get_string
#undef tobii_sdk_vector_get_string_length
#undef tobii_sdk_vector_set_int32
#undef tobii_sdk_vector_set_uint32
#undef tobii_sdk_vector_set_fixed_15x16_as_float32
#undef tobii_sdk_vector_set_fixed_22x41_as_float64
#undef tobii_sdk_vector_set_string
#undef tobii_sdk_param_stack_create
#undef tobii_sdk_param_stack_create_test_stack
#undef tobii_sdk_param_stack_clone
#undef tobii_sdk_param_stack_destroy
#undef tobii_sdk_param_stack_get_size
#undef tobii_sdk_param_stack_get_type
#undef tobii_sdk_param_stack_append
#undef tobii_sdk_param_stack_get_int32
#undef tobii_sdk_param_stack_get_uint32
#undef tobii_sdk_param_stack_get_int64
#undef tobii_sdk_param_stack_get_uint64
#undef tobii_sdk_param_stack_get_float32
#undef tobii_sdk_param_stack_get_float64
#undef tobii_sdk_param_stack_get_fixed_15x16_as_float32
#undef tobii_sdk_param_stack_get_fixed_22x41_as_float64
#undef tobii_sdk_param_stack_get_node_prolog
#undef tobii_sdk_param_stack_get_string
#undef tobii_sdk_param_stack_get_string_length
#undef tobii_sdk_param_stack_get_blob_length
#undef tobii_sdk_param_stack_get_vector
#undef tobii_sdk_param_stack_push_int32
#undef tobii_sdk_param_stack_push_uint32
#undef tobii_sdk_param_stack_push_int64
#undef tobii_sdk_param_stack_push_uint64
#undef tobii_sdk_param_stack_push_string
#undef tobii_sdk_param_stack_push_float32_as_fixed_15x16
#undef tobii_sdk_param_stack_push_float64_as_fixed_22x41
#undef tobii_sdk_param_stack_push_node_prolog
#undef tobii_sdk_param_stack_push_float32
#undef tobii_sdk_param_stack_push_float64
#undef tobii_sdk_param_stack_push_blob
#undef tobii_sdk_param_stack_push_vector
#undef tobii_sdk_callback_connection_destroy
#undef tobii_sdk_message_passer_get
#undef tobii_sdk_message_passer_destroy
#undef tobii_sdk_message_passer_enable_nop_sending
#undef tobii_sdk_message_passer_disable_nop_sending
#undef tobii_sdk_message_passer_execute_request
#undef tobii_sdk_message_passer_add_data_handler
#undef tobii_sdk_message_passer_add_error_handler
#undef tobii_sdk_clock_get_default
#undef tobii_sdk_clock_create_user_defined
#undef tobii_sdk_clock_get_time
#undef tobii_sdk_clock_get_resolution
#undef tobii_sdk_clock_destroy
#undef tobii_sdk_sync_state_clone
#undef tobii_sdk_sync_state_destroy
#undef tobii_sdk_sync_state_get_sync_state_flag
#undef tobii_sdk_sync_state_get_number_of_points_in_use
#undef tobii_sdk_sync_state_get_point_in_use
#undef tobii_sdk_sync_state_get_error_approximation
#undef tobii_sdk_sync_manager_create
#undef tobii_sdk_sync_manager_convert_from_local_to_remote
#undef tobii_sdk_sync_manager_convert_from_remote_to_local
#undef tobii_sdk_sync_manager_get_sync_state
#undef tobii_sdk_sync_manager_destroy
#undef tobii_sdk_upgrade_package_create
#undef tobii_sdk_upgrade_package_parse
#undef tobii_sdk_upgrade_package_destroy
#undef tobii_sdk_upgrade_package_get_num_parts
#undef tobii_sdk_upgrade_package_get_part
#undef tobii_sdk_upgrade_package_append_part
#undef tobii_sdk_upgrade_package_finalize
#undef tobii_sdk_upgrade_package_finalized_get_data
#undef tobii_sdk_upgrade_package_finalized_get_size
#undef tobii_sdk_upgrade_package_finalized_destroy
#undef tobii_sdk_upgrade_package_part_create_raw_part
#undef tobii_sdk_upgrade_package_part_create_legacy_metadata_part
#undef tobii_sdk_upgrade_package_part_create_legacy_file_part
#undef tobii_sdk_upgrade_package_part_create_legacy_exec_command_part
#undef tobii_sdk_upgrade_package_part_create_cancancel_part
#undef tobii_sdk_upgrade_package_part_get_representation
#undef tobii_sdk_upgrade_package_part_destroy
#undef tobii_sdk_upgrade_progress_reporter_create
#undef tobii_sdk_upgrade_progress_reporter_get_progress
#undef tobii_sdk_upgrade_progress_get_can_cancel
#undef tobii_sdk_upgrade_progress_cancel
#undef tobii_sdk_upgrade_progress_reporter_destroy
#undef tobii_sdk_upgrade_package_is_compatible_with_device
#undef tobii_sdk_upgrade_begin
#undef tobii_sdk_init
#undef tobii_sdk_free_string
#undef tobii_sdk_hmac_perform

extern "C" void (__cdecl *tobii_sdk_error_destroy)( tobii_sdk_error_t* error);
extern "C" tobii_sdk_error_t* (__cdecl *tobii_sdk_error_clone)( const tobii_sdk_error_t* error);
extern "C" const char* (__cdecl *tobii_sdk_error_get_message)( const tobii_sdk_error_t* error);
extern "C" tobii_sdk_error_code_t (__cdecl *tobii_sdk_error_get_code)( const tobii_sdk_error_t* error);
extern "C" const char* (__cdecl *tobii_sdk_error_to_string)( const tobii_sdk_error_t* error);
extern "C" uint32_t (__cdecl *tobii_sdk_error_is_failure)( const tobii_sdk_error_t* error);
extern "C" const char* (__cdecl *tobii_sdk_error_convert_code_to_string)( tobii_sdk_error_code_t code);
extern "C" tobii_sdk_error_t* (__cdecl *tobii_sdk_error_create_generic)( tobii_sdk_error_code_t code, const char* message);
extern "C" tobii_sdk_blob_t* (__cdecl *tobii_sdk_blob_create)( uint32_t size, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_blob_get_size)( tobii_sdk_blob_t* blob, tobii_sdk_error_t** error);
extern "C" uint8_t* (__cdecl *tobii_sdk_blob_get_data)( tobii_sdk_blob_t* blob, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_blob_destroy)( tobii_sdk_blob_t* blob);
extern "C" tobii_sdk_mainloop_t* (__cdecl *tobii_sdk_mainloop_create)( tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_mainloop_run)( tobii_sdk_mainloop_t* loop, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_mainloop_quit)( tobii_sdk_mainloop_t* loop, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_mainloop_destroy)( tobii_sdk_mainloop_t* loop);
extern "C" uint32_t (__cdecl *tobii_sdk_device_info_iterator_move_next)( tobii_sdk_device_info_iterator_t* iterator, tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_device_info_iterator_get_key_name)( tobii_sdk_device_info_iterator_t* iterator, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_device_info_iterator_destroy)( tobii_sdk_device_info_iterator_t* iterator);
extern "C" tobii_sdk_device_info_t* (__cdecl *tobii_sdk_device_info_clone)( const tobii_sdk_device_info_t* device_info, tobii_sdk_error_t** error);
extern "C" tobii_sdk_device_info_t* (__cdecl *tobii_sdk_device_info_create_test)( tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_device_info_get_product_id)( tobii_sdk_device_info_t* device_info, tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_device_info_get)( tobii_sdk_device_info_t* device_info, const char* key, tobii_sdk_error_t** error);
extern "C" tobii_sdk_device_info_iterator_t* (__cdecl *tobii_sdk_device_info_get_iterator)( tobii_sdk_device_info_t* device_info, tobii_sdk_error_t** error);
extern "C" tobii_sdk_factory_info_t* (__cdecl *tobii_sdk_device_info_get_factory_info)( tobii_sdk_device_info_t* device_info, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_device_info_destroy)( tobii_sdk_device_info_t* device_info);
extern "C" tobii_sdk_factory_info_t* (__cdecl *tobii_sdk_factory_info_create_for_networked_eyetracker)( const char* ip_address_or_hostname, uint32_t tetserver_port, uint32_t synchronization_port, tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_factory_info_get_representation)( tobii_sdk_factory_info_t* factory_info, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_factory_info_destroy)( tobii_sdk_factory_info_t* factory_info);
extern "C" tobii_sdk_device_browser_t* (__cdecl *tobii_sdk_device_browser_create)( tobii_sdk_mainloop_t* mainloop, tobii_sdk_device_browser_callback_t callback, void* user_data, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_device_browser_destroy)( tobii_sdk_device_browser_t* browser);
extern "C" tobii_sdk_device_info_list_t* (__cdecl *tobii_sdk_device_browser_get_device_list)( tobii_sdk_device_browser_t* browser, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_device_info_list_get_size)( tobii_sdk_device_info_list_t* device_list, tobii_sdk_error_t** error);
extern "C" tobii_sdk_device_info_t* (__cdecl *tobii_sdk_device_info_list_get_at_index)( tobii_sdk_device_info_list_t* list, uint32_t index, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_device_info_list_destroy)( tobii_sdk_device_info_list_t* device_list);
extern "C" uint32_t (__cdecl *tobii_sdk_vector_type_is_compatible)( tobii_sdk_param_type_t element_type);
extern "C" tobii_sdk_vector_t* (__cdecl *tobii_sdk_vector_create)( uint32_t size, tobii_sdk_param_type_t element_type, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_vector_destroy)( tobii_sdk_vector_t* vector);
extern "C" tobii_sdk_param_type_t (__cdecl *tobii_sdk_vector_get_element_type)( const tobii_sdk_vector_t* vector, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_vector_get_length)( const tobii_sdk_vector_t* vector, tobii_sdk_error_t** error);
extern "C" int32_t (__cdecl *tobii_sdk_vector_get_int32)( const tobii_sdk_vector_t* vector, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_vector_get_uint32)( const tobii_sdk_vector_t* vector, uint32_t index, tobii_sdk_error_t** error);
extern "C" float (__cdecl *tobii_sdk_vector_get_fixed_15x16_as_float32)( const tobii_sdk_vector_t* vector, uint32_t index, tobii_sdk_error_t** error);
extern "C" double (__cdecl *tobii_sdk_vector_get_fixed_22x41_as_float64)( const tobii_sdk_vector_t* vector, uint32_t index, tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_vector_get_string)( const tobii_sdk_vector_t* vector, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_vector_get_string_length)( const tobii_sdk_vector_t* vector, uint32_t index, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_vector_set_int32)( tobii_sdk_vector_t* vector, uint32_t index, int32_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_vector_set_uint32)( tobii_sdk_vector_t* vector, uint32_t index, uint32_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_vector_set_fixed_15x16_as_float32)( tobii_sdk_vector_t* vector, uint32_t index, float value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_vector_set_fixed_22x41_as_float64)( tobii_sdk_vector_t* vector, uint32_t index, double value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_vector_set_string)( tobii_sdk_vector_t* vector, uint32_t index, const char* string, uint32_t length, tobii_sdk_error_t** error);
extern "C" tobii_sdk_param_stack_t* (__cdecl *tobii_sdk_param_stack_create)( tobii_sdk_error_t** error);
extern "C" tobii_sdk_param_stack_t* (__cdecl *tobii_sdk_param_stack_create_test_stack)( tobii_sdk_error_t** error);
extern "C" tobii_sdk_param_stack_t* (__cdecl *tobii_sdk_param_stack_clone)( const tobii_sdk_param_stack_t* param, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_destroy)( tobii_sdk_param_stack_t* param);
extern "C" uint32_t (__cdecl *tobii_sdk_param_stack_get_size)( const tobii_sdk_param_stack_t* param, tobii_sdk_error_t** error);
extern "C" tobii_sdk_param_type_t (__cdecl *tobii_sdk_param_stack_get_type)( const tobii_sdk_param_stack_t* param, uint32_t index, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_append)( tobii_sdk_param_stack_t* param_stack, const tobii_sdk_param_stack_t* append, tobii_sdk_error_t** error);
extern "C" int32_t (__cdecl *tobii_sdk_param_stack_get_int32)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_param_stack_get_uint32)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" int64_t (__cdecl *tobii_sdk_param_stack_get_int64)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint64_t (__cdecl *tobii_sdk_param_stack_get_uint64)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" float (__cdecl *tobii_sdk_param_stack_get_float32)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" double (__cdecl *tobii_sdk_param_stack_get_float64)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" float (__cdecl *tobii_sdk_param_stack_get_fixed_15x16_as_float32)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" double (__cdecl *tobii_sdk_param_stack_get_fixed_22x41_as_float64)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_param_stack_get_node_prolog)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_param_stack_get_string)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_param_stack_get_string_length)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_param_stack_get_blob_length)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" tobii_sdk_vector_t* (__cdecl *tobii_sdk_param_stack_get_vector)( const tobii_sdk_param_stack_t* params, uint32_t index, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_int32)( tobii_sdk_param_stack_t* params, int32_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_uint32)( tobii_sdk_param_stack_t* params, uint32_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_int64)( tobii_sdk_param_stack_t* params, int64_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_uint64)( tobii_sdk_param_stack_t* params, uint64_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_string)( tobii_sdk_param_stack_t* params, const char* value, const uint32_t length, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_float32_as_fixed_15x16)( tobii_sdk_param_stack_t* params, float value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_float64_as_fixed_22x41)( tobii_sdk_param_stack_t* params, double value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_node_prolog)( tobii_sdk_param_stack_t* params, uint32_t value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_float32)( tobii_sdk_param_stack_t* params, float value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_float64)( tobii_sdk_param_stack_t* params, double value, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_blob)( tobii_sdk_param_stack_t* params, const unsigned char* blob, const uint32_t length, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_param_stack_push_vector)( tobii_sdk_param_stack_t* params, const tobii_sdk_vector_t* vector, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_callback_connection_destroy)( tobii_sdk_callback_connection_t* callback_connection);
extern "C" void (__cdecl *tobii_sdk_message_passer_get)( tobii_sdk_factory_info_t* factory_info, tobii_sdk_mainloop_t* loop, tobii_sdk_message_passer_get_handler_t handler, void* handler_user_data, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_message_passer_destroy)( tobii_sdk_message_passer_t* mpi);
extern "C" void (__cdecl *tobii_sdk_message_passer_enable_nop_sending)( tobii_sdk_message_passer_t* mpi, tobii_sdk_opcode_t opcode, const tobii_sdk_param_stack_t* params, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_message_passer_disable_nop_sending)( tobii_sdk_message_passer_t* mpi, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_message_passer_execute_request)( tobii_sdk_message_passer_t* mpi, tobii_sdk_opcode_t opcode, const tobii_sdk_param_stack_t* params, tobii_sdk_response_handler_t response_handler, void* response_user_data, tobii_sdk_error_t** error);
extern "C" tobii_sdk_callback_connection_t* (__cdecl *tobii_sdk_message_passer_add_data_handler)( tobii_sdk_message_passer_t* mpi, tobii_sdk_opcode_t opcode, tobii_sdk_data_handler_t data_handler, void* user_data, tobii_sdk_error_t** error);
extern "C" tobii_sdk_callback_connection_t* (__cdecl *tobii_sdk_message_passer_add_error_handler)( tobii_sdk_message_passer_t* mpi, tobii_sdk_error_handler_t error_handler, void* user_data, tobii_sdk_error_t** error);
extern "C" tobii_sdk_clock_t* (__cdecl *tobii_sdk_clock_get_default)( tobii_sdk_error_t** error);
extern "C" tobii_sdk_clock_t* (__cdecl *tobii_sdk_clock_create_user_defined)( tobii_sdk_clock_get_time_fn_t time_func, tobii_sdk_clock_get_resolution_fn_t res_func, tobii_sdk_clock_destroy_fn_t destructor, void* instance_data, tobii_sdk_error_t** error);
extern "C" int64_t (__cdecl *tobii_sdk_clock_get_time)( tobii_sdk_clock_t* clock, tobii_sdk_error_t** error);
extern "C" int64_t (__cdecl *tobii_sdk_clock_get_resolution)( tobii_sdk_clock_t* clock, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_clock_destroy)( tobii_sdk_clock_t* clock);
extern "C" tobii_sdk_sync_state_t* (__cdecl *tobii_sdk_sync_state_clone)( const tobii_sdk_sync_state_t* state, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_sync_state_destroy)( tobii_sdk_sync_state_t* state);
extern "C" tobii_sdk_sync_state_flag_t (__cdecl *tobii_sdk_sync_state_get_sync_state_flag)( const tobii_sdk_sync_state_t* state, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_sync_state_get_number_of_points_in_use)( const tobii_sdk_sync_state_t* state, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_sync_state_get_point_in_use)( const tobii_sdk_sync_state_t* state, uint32_t index, int64_t* local_midpoint, int64_t* remote, int64_t* roundtrip, tobii_sdk_error_t** error);
extern "C" int64_t (__cdecl *tobii_sdk_sync_state_get_error_approximation)( const tobii_sdk_sync_state_t* state, tobii_sdk_error_t** error);
extern "C" tobii_sdk_sync_manager_t* (__cdecl *tobii_sdk_sync_manager_create)( tobii_sdk_clock_t* clock, tobii_sdk_factory_info_t* factory_info, tobii_sdk_mainloop_t* loop, tobii_sdk_sync_manager_error_handler_t error_handler, void* error_handler_user_data, tobii_sdk_sync_manager_status_changed_handler_t status_handler, void* status_changed_user_data, tobii_sdk_error_t** error);
extern "C" int64_t (__cdecl *tobii_sdk_sync_manager_convert_from_local_to_remote)( const tobii_sdk_sync_manager_t* sync_manager, int64_t local, tobii_sdk_error_t** error);
extern "C" int64_t (__cdecl *tobii_sdk_sync_manager_convert_from_remote_to_local)( const tobii_sdk_sync_manager_t* sync_manager, int64_t remote, tobii_sdk_error_t** error);
extern "C" tobii_sdk_sync_state_t* (__cdecl *tobii_sdk_sync_manager_get_sync_state)( const tobii_sdk_sync_manager_t* sync_manager, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_sync_manager_destroy)( tobii_sdk_sync_manager_t* sync_manager);
extern "C" tobii_sdk_upgrade_package_t* (__cdecl *tobii_sdk_upgrade_package_create)( tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_t* (__cdecl *tobii_sdk_upgrade_package_parse)( const void* package, uint32_t package_size, uint32_t allow_unknown_parts, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_package_destroy)( tobii_sdk_upgrade_package_t* package);
extern "C" uint32_t (__cdecl *tobii_sdk_upgrade_package_get_num_parts)( const tobii_sdk_upgrade_package_t* package, tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_part_t* (__cdecl *tobii_sdk_upgrade_package_get_part)( tobii_sdk_upgrade_package_t* package, uint32_t index, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_package_append_part)( tobii_sdk_upgrade_package_t* package, tobii_sdk_upgrade_package_part_t* part, tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_finalized_t* (__cdecl *tobii_sdk_upgrade_package_finalize)( tobii_sdk_upgrade_package_t* package, tobii_sdk_error_t** error);
extern "C" const uint8_t* (__cdecl *tobii_sdk_upgrade_package_finalized_get_data)( const tobii_sdk_upgrade_package_finalized_t* finalized, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_upgrade_package_finalized_get_size)( const tobii_sdk_upgrade_package_finalized_t* finalized, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_package_finalized_destroy)( tobii_sdk_upgrade_package_finalized_t* finalized);
extern "C" tobii_sdk_upgrade_package_part_t* (__cdecl *tobii_sdk_upgrade_package_part_create_raw_part)( uint32_t type, uint32_t flags, uint32_t size, uint32_t id, uint32_t order, const uint8_t* payload, tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_part_t* (__cdecl *tobii_sdk_upgrade_package_part_create_legacy_metadata_part)( uint32_t flags, uint32_t id, uint32_t order, const char* key, const char* value, tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_part_t* (__cdecl *tobii_sdk_upgrade_package_part_create_legacy_file_part)( uint32_t flags, uint32_t id, uint32_t order, const char* target_path, const uint8_t* file_contents, uint32_t file_size, tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_part_t* (__cdecl *tobii_sdk_upgrade_package_part_create_legacy_exec_command_part)( uint32_t flags, uint32_t id, uint32_t order, uint32_t exec_flags, const char* command_line, tobii_sdk_error_t** error);
extern "C" tobii_sdk_upgrade_package_part_t* (__cdecl *tobii_sdk_upgrade_package_part_create_cancancel_part)( uint32_t flags, uint32_t id, uint32_t order, uint32_t cancancel, tobii_sdk_error_t** error);
extern "C" const char* (__cdecl *tobii_sdk_upgrade_package_part_get_representation)( const tobii_sdk_upgrade_package_part_t* part, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_package_part_destroy)( tobii_sdk_upgrade_package_part_t* part);
extern "C" tobii_sdk_upgrade_progress_reporter_t* (__cdecl *tobii_sdk_upgrade_progress_reporter_create)( tobii_sdk_upgrade_completed_handler_t completion_handler, tobii_sdk_upgrade_progress_handler_t progress_handler, tobii_sdk_upgrade_cancancel_changed_handler_t cancancel_handler, void* user_data, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_progress_reporter_get_progress)( tobii_sdk_upgrade_progress_reporter_t* progress_reporter, uint32_t* current_step, uint32_t* number_of_steps, uint32_t* percent, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_upgrade_progress_get_can_cancel)( tobii_sdk_upgrade_progress_reporter_t* progress_reporter, tobii_sdk_error_t** error);
extern "C" uint32_t (__cdecl *tobii_sdk_upgrade_progress_cancel)( tobii_sdk_upgrade_progress_reporter_t* progress_reporter, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_progress_reporter_destroy)( tobii_sdk_upgrade_progress_reporter_t* progress_reporter);
extern "C" uint32_t (__cdecl *tobii_sdk_upgrade_package_is_compatible_with_device)( tobii_sdk_mainloop_t* mainloop, const tobii_sdk_upgrade_package_t* package, const tobii_sdk_device_info_t* device, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_upgrade_begin)( tobii_sdk_mainloop_t* mainloop, const tobii_sdk_upgrade_package_t* package, const tobii_sdk_device_info_t* device, tobii_sdk_upgrade_progress_reporter_t* progress_reporter, tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_init)( tobii_sdk_error_t** error);
extern "C" void (__cdecl *tobii_sdk_free_string)( const char* str);
extern "C" tobii_sdk_blob_t* (__cdecl *tobii_sdk_hmac_perform)( uint32_t algorithm, const char* key, uint32_t key_length, const char* input, uint32_t input_length, tobii_sdk_error_t** error);

namespace Dylib { bool tetio_Loaded(); }

#endif // core_IMPORTS_H
