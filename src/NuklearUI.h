#pragma once

int NuklearInit(GLFWwindow* win, struct nk_glfw* glfw, struct nk_context** context, struct nk_colorf* background, struct nk_font_atlas* atlas)
{
    *context = nk_glfw3_init(glfw, win, NK_GLFW3_INSTALL_CALLBACKS);

    if (*context == NULL) {
	fprintf(stderr, "nk_glfw3_init() FAILED\n");
	return -1;
    }
    {
	nk_glfw3_font_stash_begin(glfw, &atlas);
	nk_glfw3_font_stash_end(glfw);
    }

    background->r = 0.2f;
    background->g = 0.2f;
    background->b = 0.2f;
    background->a = 1.0f;

    set_style(*context, THEME_BYK);
    (*context)->style.window.rounding = 10.0f;
    (*context)->style.button.rounding = 10.0f;
    (*context)->style.combo.rounding = 10.0f;
    (*context)->style.edit.rounding = 10.0f;
    (*context)->style.property.rounding = 3.75f;
    (*context)->style.property.inc_button.rounding = 3.75f;
    (*context)->style.property.dec_button.rounding = 3.75f;
    (*context)->style.property.hover = nk_style_item_color(nk_rgba(40, 66, 140, 220));
    (*context)->style.property.edit.hover = nk_style_item_color(nk_rgba(40, 66, 140, 220));
    return 0;
}

void NuklearWidget(struct nk_context* context, unsigned short numInstances, float posX, float posY, vec3* tVector, vec3* rVector, float* deg)
{
    if (nk_begin(context,
		 "Shape Control.",
		 nk_rect(posX, posY, 467, 164),
		 NK_WINDOW_BORDER |
		 NK_WINDOW_MOVABLE |
		 NK_WINDOW_SCALABLE |
		 NK_WINDOW_MINIMIZABLE |
		 NK_WINDOW_TITLE))
    {
	    for (int i = 0; i < numInstances; i++)
	    {
		nk_layout_row_static(context, 1.5f, 0, 0);
		nk_layout_row_dynamic(context, 12.5f, 4);
	
		context->style.property.inc_button.normal = nk_style_item_color(nk_rgb(192, 0, 0));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgb(192, 32, 0));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgb(192, 32, 0));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgb(192, 0, 0));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgb(192, 32, 0));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgb(192, 32, 0));
		nk_property_float(context, "#TX:", -10.0, &tVector[i][0], 10.0f, 0.05f, 0.25);

		context->style.property.inc_button.normal = nk_style_item_color(nk_rgb(0, 192, 0));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgb(32, 192, 0));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgb(32, 192, 0));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgb(0, 192, 0));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgb(32, 192, 0));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgb(32, 192, 0));
		nk_property_float(context, "#TY:", -10.0, &tVector[i][1], 10.0f, 0.05f, 0.25);

		context->style.property.inc_button.normal = nk_style_item_color(nk_rgb(0, 0, 192));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgb(32, 0, 192));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgb(32, 0, 192));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgb(0, 0, 192));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgb(32, 0, 192));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgb(32, 0, 192));
		nk_property_float(context, "#TZ:", -10.0, &tVector[i][2], 10.0f, 0.05f, 0.25);
		nk_label(context, "Translation.", NK_TEXT_LEFT);

		nk_layout_row_static(context, 1.5f, 0, 0);

		nk_layout_row_dynamic(context, 12.5f, 4);

		context->style.property.inc_button.normal = nk_style_item_color(nk_rgb(192, 0, 0));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgb(192, 32, 0));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgb(192, 32, 0));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgb(192, 0, 0));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgb(192, 32, 0));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgb(192, 32, 0));
		nk_property_float(context, "#RX:", -1.0, &rVector[i][0], 1.0f, 0.05f, 0.125);

		context->style.property.inc_button.normal = nk_style_item_color(nk_rgb(0, 192, 0));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgb(32, 192, 0));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgb(32, 192, 0));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgb(0, 192, 0));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgb(32, 192, 0));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgb(32, 192, 0));
		nk_property_float(context, "#RY:", -1.0, &rVector[i][1], 1.0f, 0.05f, 0.125);

		context->style.property.inc_button.normal = nk_style_item_color(nk_rgb(0, 0, 192));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgb(32, 0, 192));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgb(32, 0, 192));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgb(0, 0, 192));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgb(32, 0, 192));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgb(32, 0, 192));
		nk_property_float(context, "#RZ:", -1.0, &rVector[i][2], 1.0f, 0.05f, 0.125);
		nk_label(context, "Rotation.", NK_TEXT_LEFT);
		nk_layout_row_dynamic(context, 12.5f, 1);

		context->style.property.inc_button.normal = nk_style_item_color(nk_rgba(30, 56, 87, 192));
		context->style.property.inc_button.active = nk_style_item_color(nk_rgba(40, 66, 140, 220));
		context->style.property.inc_button.hover = nk_style_item_color(nk_rgba(40, 66, 140, 220));
		context->style.property.dec_button.normal = nk_style_item_color(nk_rgba(30, 56, 87, 192));
		context->style.property.dec_button.active = nk_style_item_color(nk_rgba(40, 66, 140, 220));
		context->style.property.dec_button.hover = nk_style_item_color(nk_rgba(40, 66, 140, 220));
		nk_property_float(context, "#Deg:", -360.0f, &deg[i], 360.0f, 1.0f, 1);

		nk_layout_row_static(context, 1.5f, 0, 0);
	    }
    }
    nk_end(context);
}
