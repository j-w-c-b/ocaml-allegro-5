#include "al5.h"

#include <allegro5/allegro_native_dialog.h>

CAMLprim value ml_al_init_native_dialog_addon(value unit)
{
    CAMLparam1(unit);
    if (!al_init_native_dialog_addon()) {
        caml_failwith("al_init_native_file_dialog");
    }
    CAMLreturn(Val_unit);
}

ml_function_noarg_ret(al_is_native_dialog_addon_initialized, Val_bool)

ml_function_noarg(al_shutdown_native_dialog_addon)

ml_function_noarg_ret(al_get_allegro_native_dialog_version, Val_int)

enum {
    ML_FILECHOOSER_FILE_MUST_EXIST = 1 << 0,
    ML_FILECHOOSER_SAVE = 1 << 1,
    ML_FILECHOOSER_FOLDER = 1 << 2,
    ML_FILECHOOSER_PICTURES = 1 << 3,
    ML_FILECHOOSER_SHOW_HIDDEN = 1 << 4,
    ML_FILECHOOSER_MULTIPLE = 1 << 5,
};

static int const filechooser_mode_conv[][2] = {
    { ML_FILECHOOSER_FILE_MUST_EXIST, ALLEGRO_FILECHOOSER_FILE_MUST_EXIST },
    { ML_FILECHOOSER_SAVE, ALLEGRO_FILECHOOSER_SAVE },
    { ML_FILECHOOSER_FOLDER, ALLEGRO_FILECHOOSER_FOLDER },
    { ML_FILECHOOSER_PICTURES, ALLEGRO_FILECHOOSER_PICTURES },
    { ML_FILECHOOSER_SHOW_HIDDEN, ALLEGRO_FILECHOOSER_SHOW_HIDDEN },
    { ML_FILECHOOSER_MULTIPLE, ALLEGRO_FILECHOOSER_MULTIPLE },
};

CAMLprim value ml_al_create_native_file_dialog(value initial_path, value title, value patterns, value mode)
{
    CAMLparam4(initial_path, title, patterns, mode);
    int c_mode = convert_flags(Int_val(mode), filechooser_mode_conv, 0);
    ALLEGRO_FILECHOOSER *dialog = al_create_native_file_dialog(String_val(initial_path), String_val(title), String_val(patterns), c_mode);
    CAMLreturn(Val_ptr(dialog));
}

CAMLprim value ml_al_show_native_file_dialog(value display, value dialog)
{
    bool success;
    CAMLparam2(display, dialog);
    if (Is_none(display)) {
        success = al_show_native_file_dialog(NULL, Ptr_val(dialog));
    } else {
        success = al_show_native_file_dialog(Ptr_val(Some_val(display)), Ptr_val(dialog));
    }
    CAMLreturn(Val_bool(success));
}

ml_function_1arg_ret(al_get_native_file_dialog_count, Ptr_val, Val_int)

CAMLprim value ml_al_get_native_file_dialog_path(value dialog, value i)
{
    CAMLparam2(dialog, i);
    CAMLlocal1(path);
    const char *c_path = al_get_native_file_dialog_path(Ptr_val(dialog), Long_val(i));
    path = caml_copy_string(c_path);
    CAMLreturn(path);
}

ml_function_1arg(al_destroy_native_file_dialog, Ptr_val)

enum {
    ML_MESSAGEBOX_WARN = 1 << 0,
    ML_MESSAGEBOX_ERROR = 1 << 1,
    ML_MESSAGEBOX_QUESTION = 1 << 2,
    ML_MESSAGEBOX_OK_CANCEL = 1 << 3,
    ML_MESSAGEBOX_YES_NO = 1 << 4,
};

static int const messagebox_flags_conv[][2] = {
    { ML_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_WARN },
    { ML_MESSAGEBOX_ERROR , ALLEGRO_MESSAGEBOX_ERROR },
    { ML_MESSAGEBOX_QUESTION , ALLEGRO_MESSAGEBOX_QUESTION },
    { ML_MESSAGEBOX_OK_CANCEL , ALLEGRO_MESSAGEBOX_OK_CANCEL },
    { ML_MESSAGEBOX_YES_NO , ALLEGRO_MESSAGEBOX_YES_NO },
};

CAMLprim value ml_al_show_native_message_box(value display, value title, value heading, value text, value buttons, value flags)
{
    CAMLparam5(display, title, heading, text, buttons);
    CAMLxparam1(flags);
    int result;
    ALLEGRO_DISPLAY *c_display;
    const char *c_buttons;
    if (Is_none(display)) {
        c_display = NULL;
    } else {
        c_display = Ptr_val(Some_val(display));
    }
    if (Is_none(buttons)) {
        c_buttons = NULL;
    } else {
        c_buttons = String_val(Some_val(buttons));
    }
    int c_flags = convert_flags(Int_val(flags), messagebox_flags_conv, 0);

    result = al_show_native_message_box(c_display, String_val(title), String_val(heading), String_val(text), c_buttons, c_flags);
    CAMLreturn(Val_int(result));
}

CAMLprim value ml_al_show_native_message_box_bytecode(value *argv, int argc)
{
    return ml_al_show_native_message_box(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
}

ml_function_noarg_ret(al_create_menu, Val_ptr)

ml_function_noarg_ret(al_create_popup_menu, Val_ptr)

enum {
    ML_MENU_ITEM_DISABLED = 1 << 0,
    ML_MENU_ITEM_CHECKBOX = 1 << 1,
    ML_MENU_ITEM_CHECKED = 1 << 2,
};

static int const menu_item_flags_conv[][2] = {
    { ML_MENU_ITEM_DISABLED, ALLEGRO_MENU_ITEM_DISABLED },
    { ML_MENU_ITEM_CHECKBOX , ALLEGRO_MENU_ITEM_CHECKBOX },
    { ML_MENU_ITEM_CHECKED , ALLEGRO_MENU_ITEM_CHECKED },
};

CAMLprim value ml_al_append_menu_item(value parent, value title, value id, value flags, value icon, value submenu)
{
    CAMLparam5(parent, title, id, flags, icon);
    CAMLxparam1(submenu);
    ALLEGRO_BITMAP *c_icon;
    ALLEGRO_MENU *c_submenu;
    if (Is_none(icon)) {
        c_icon = NULL;
    } else {
        c_icon = Ptr_val(Some_val(icon));
    }

    if (Is_none(submenu)) {
        c_submenu = NULL;
    } else {
        c_submenu = Ptr_val(Some_val(submenu));
    }

    int c_flags = convert_flags(Int_val(flags), menu_item_flags_conv, 0);
    int result = al_append_menu_item(Ptr_val(parent), String_val(title), Int_val(id), c_flags, c_icon, c_submenu);

    CAMLreturn(Val_int(result));
}

CAMLprim value ml_al_append_menu_item_bytecode(value *argv, int argc)
{
    return ml_al_append_menu_item(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
}

CAMLprim value ml_al_insert_menu_item(value parent, value pos, value title, value id, value flags, value icon, value submenu)
{
    CAMLparam5(parent, pos, title, id, flags);
    CAMLxparam2(icon, submenu);
    int c_flags = convert_flags(Int_val(flags), menu_item_flags_conv, 0);
    int result = al_insert_menu_item(Ptr_val(parent), Int_val(pos), String_val(title), Int_val(id), c_flags, Ptr_val(icon), Ptr_val(submenu));
    CAMLreturn(Val_int(result));
}

CAMLprim value ml_al_insert_menu_item_bytecode(value *argv, int argc)
{
    return ml_al_insert_menu_item(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
}

CAMLprim value ml_al_remove_menu_item(value menu, value pos)
{
    CAMLparam2(menu, pos);
    bool success = al_remove_menu_item(Ptr_val(menu), Int_val(pos));
    CAMLreturn(Val_bool(success));
}

ml_function_1arg_ret(al_clone_menu, Ptr_val, Val_ptr)

ml_function_1arg_ret(al_clone_menu_for_popup, Ptr_val, Val_ptr)

ml_function_1arg(al_destroy_menu, Ptr_val);

CAMLprim value ml_al_get_menu_item_caption(value menu, value pos)
{
    CAMLparam2(menu, pos);
    CAMLlocal1(caption);
    const char *c_caption = al_get_menu_item_caption(Ptr_val(menu), Int_val(pos));
    caption = caml_copy_string(c_caption);
    CAMLreturn(caption);
}

CAMLprim value ml_al_set_menu_item_caption(value menu, value pos, value caption)
{
    CAMLparam3(menu, pos, caption);
    al_set_menu_item_caption(Ptr_val(menu), Int_val(pos), String_val(caption));
    CAMLreturn(Val_unit);
}

CAMLprim value ml_al_get_menu_item_flags(value menu, value pos)
{
    CAMLparam2(menu, pos);
    int c_flags = al_get_menu_item_flags(Ptr_val(menu), Int_val(pos));
    int flags = convert_flags(c_flags, menu_item_flags_conv, 1);
    CAMLreturn(Val_int(flags));
}

CAMLprim value ml_al_set_menu_item_flags(value menu, value pos, value flags)
{
    CAMLparam3(menu, pos, flags);
    int c_flags = convert_flags(flags, menu_item_flags_conv, 0);
    al_set_menu_item_flags(Ptr_val(menu), Int_val(pos), c_flags);

    CAMLreturn(Val_unit);
}

CAMLprim value ml_al_get_menu_item_icon(value menu, value pos)
{
    CAMLparam2(menu, pos);
    ALLEGRO_BITMAP *icon = al_get_menu_item_icon(Ptr_val(menu), Int_val(pos));
    if (icon != NULL) {
        CAMLreturn(caml_alloc_some(Val_ptr(icon)));
    }
    CAMLreturn(Val_none);
}

CAMLprim value ml_al_set_menu_item_icon(value menu, value pos, value icon)
{
    CAMLparam3(menu, pos, icon);
    al_set_menu_item_icon(Ptr_val(menu), Int_val(pos), Ptr_val(icon));

    CAMLreturn(Val_unit);
}

CAMLprim value ml_al_find_menu(value haystack, value id)
{
    CAMLparam2(haystack, id);
    ALLEGRO_MENU *menu = al_find_menu(Ptr_val(haystack), Int_val(id));
    if (menu != NULL) {
        CAMLreturn(caml_alloc_some(Val_ptr(menu)));
    }
    CAMLreturn(Val_none);
}

CAMLprim value ml_al_find_menu_item(value haystack, value id)
{
    CAMLparam2(haystack, id);
    ALLEGRO_MENU *menu;
    int index;
    CAMLlocal1(results);
    bool success = al_find_menu_item(Ptr_val(haystack), Int_val(id), &menu, &index);
    if (success) {
        results = caml_alloc_tuple(2);
        Store_field(results, 0, Val_ptr(menu));
        Store_field(results, 1, Val_int(index));
        CAMLreturn(caml_alloc_some(results));
    }
    CAMLreturn(Val_none);
}

ml_function_noarg_ret(al_get_default_menu_event_source, Val_ptr);

ml_function_1arg_ret(al_enable_menu_event_source, Ptr_val, Val_ptr);

ml_function_1arg(al_disable_menu_event_source, Ptr_val);

CAMLprim value ml_al_get_display_menu(value display)
{
    CAMLparam1(display);
    ALLEGRO_MENU *menu = al_get_display_menu(Ptr_val(display));
    if (menu != NULL) {
        CAMLreturn(caml_alloc_some(Val_ptr(menu)));
    }
    CAMLreturn(Val_none);
}

CAMLprim value ml_al_set_display_menu(value display, value menu)
{
    CAMLparam2(display, menu);
    bool success;
    ALLEGRO_MENU *c_menu;
    if (Is_none(menu)) {
        c_menu = NULL;
    } else {
        c_menu = Ptr_val(Some_val(menu));
    }
    success = al_set_display_menu(Ptr_val(display), c_menu);
    CAMLreturn(Val_bool(success));
}

CAMLprim value ml_al_popup_menu(value popup, value display)
{
    CAMLparam2(popup, display);
    bool success = al_popup_menu(Ptr_val(popup), Ptr_val(display));
    CAMLreturn(Val_bool(success));
}

ml_function_1arg_ret(al_remove_display_menu, Ptr_val, Val_ptr);
