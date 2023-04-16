#pragma once

#include "gui_common_elements.hpp"
#include "gui_element_types.hpp"

namespace ui {

class ledger_page_number {
public:
    int8_t value;
};

class ledger_prev_button : public generic_settable_element<button_element_base, ledger_page_number> {
public:
    void button_action(sys::state& state) noexcept override {
        if(parent) {
            auto num = int8_t(content.value - 1);
            if(num <= 0)
                num = 11;
            Cyto::Any new_payload = ledger_page_number{ num };
            parent->impl_set(state, new_payload);
        }
    }
};

class ledger_next_button : public generic_settable_element<button_element_base, ledger_page_number> {
public:
    void button_action(sys::state& state) noexcept override {
        if(parent) {
            auto num = int8_t(content.value + 1);
            if(num > 11)
                num = 1;
            Cyto::Any new_payload = ledger_page_number{ num };
            parent->impl_set(state, new_payload);
        }
    }
};

class ledger_nation_ranking_entry : public listbox_row_element_base<dcon::nation_id> {
    flag_button* country_flag = nullptr;
public:
    void on_create(sys::state& state) noexcept override {
        listbox_row_element_base::on_create(state);

        xy_pair cell_offset{ 0, 0 };
        auto apply_offset = [&](auto& ptr) {
            ptr->base_data.position = cell_offset;
            cell_offset.x += ptr->base_data.size.x;
        };
        // Country flag
        {
            auto ptr = make_element_by_type<flag_button>(state, state.ui_state.defs_by_name.find("ledger_default_flag")->second.definition);
            country_flag = ptr.get();
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
        // Country name
        {
            auto ptr = make_element_by_type<generic_name_text<dcon::nation_id>>(state, state.ui_state.defs_by_name.find("ledger_default_textbox")->second.definition);
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
        // Country status
        {
            auto ptr = make_element_by_type<nation_status_text>(state, state.ui_state.defs_by_name.find("ledger_default_textbox")->second.definition);
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
        // Military score
        {
            auto ptr = make_element_by_type<nation_military_score_text>(state, state.ui_state.defs_by_name.find("ledger_default_textbox")->second.definition);
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
        // Industrial score
        {
            auto ptr = make_element_by_type<nation_industry_score_text>(state, state.ui_state.defs_by_name.find("ledger_default_textbox")->second.definition);
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
        // Prestige
        {
            auto ptr = make_element_by_type<nation_prestige_text>(state, state.ui_state.defs_by_name.find("ledger_default_textbox")->second.definition);
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
        // Total score
        {
            auto ptr = make_element_by_type<nation_total_score_text>(state, state.ui_state.defs_by_name.find("ledger_default_textbox")->second.definition);
            apply_offset(ptr);
            add_child_to_front(std::move(ptr));
        }
    }

    void update(sys::state& state) noexcept override {
        country_flag->on_update(state);
        Cyto::Any payload = content;
        impl_set(state, payload);
    }
};
class ledger_nation_ranking_listbox : public listbox_element_base<ledger_nation_ranking_entry, dcon::nation_id> {
protected:
    std::string_view get_row_element_name() override {
        return "default_listbox_entry";
    }
public:
    void on_update(sys::state& state) noexcept override {
        row_contents.clear();
        state.world.for_each_nation([&](dcon::nation_id id) {
            row_contents.push_back(id);
        });
        update(state);
    }
};

class ledger_window : public window_element_base {
    window_element_base* commodity_linegraph = nullptr;
    window_element_base* commodity_linegraph_legend = nullptr;
    image_element_base* commodity_linegraph_image = nullptr;
    simple_text_element_base* page_number_text = nullptr;
    simple_text_element_base* ledger_header_text = nullptr;
    ledger_page_number page_num{ int8_t(1) };

    ledger_nation_ranking_listbox* nation_ranking_listbox = nullptr;
    ledger_nation_ranking_listbox* nation_compare_listbox = nullptr;
    ledger_nation_ranking_listbox* nation_party_listbox = nullptr;
    ledger_nation_ranking_listbox* nation_political_reforms_listbox = nullptr;
    ledger_nation_ranking_listbox* nation_social_reforms_listbox = nullptr;
    ledger_nation_ranking_listbox* nation_pops_listbox = nullptr;
    ledger_nation_ranking_listbox* provinces_listbox = nullptr;
    ledger_nation_ranking_listbox* provinces_pops_listbox = nullptr;
    ledger_nation_ranking_listbox* provinces_production_listbox = nullptr;
    ledger_nation_ranking_listbox* factory_production_listbox = nullptr;

    void hide_sub_windows(sys::state& state) noexcept {
        nation_ranking_listbox->set_visible(state, false);
        nation_compare_listbox->set_visible(state, false);
        nation_party_listbox->set_visible(state, false);
        nation_political_reforms_listbox->set_visible(state, false);
        nation_social_reforms_listbox->set_visible(state, false);
        nation_pops_listbox->set_visible(state, false);
        provinces_listbox->set_visible(state, false);
        provinces_pops_listbox->set_visible(state, false);
        provinces_production_listbox->set_visible(state, false);
        factory_production_listbox->set_visible(state, false);

        // Linegraphs
        commodity_linegraph->set_visible(state, false);
        commodity_linegraph_legend->set_visible(state, false);
        commodity_linegraph_image->set_visible(state, false);
    }
public:
    void on_create(sys::state& state) noexcept override {
        window_element_base::on_create(state);
        
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            nation_ranking_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            nation_compare_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            nation_party_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            nation_political_reforms_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            nation_social_reforms_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            nation_pops_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            provinces_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            provinces_pops_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            provinces_production_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }
        {
            auto ptr = make_element_by_type<ledger_nation_ranking_listbox>(state, state.ui_state.defs_by_name.find("default_listbox")->second.definition);
            factory_production_listbox = ptr.get();
            add_child_to_front(std::move(ptr));
        }

        Cyto::Any payload = page_num;
        impl_set(state, payload);
    }

    std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
        if(name == "close") {
            return make_element_by_type<generic_close_button>(state, id);
        } else if(name == "ledger_linegraphs") {
            auto ptr = make_element_by_type<window_element_base>(state, id);
            commodity_linegraph = ptr.get();
            return ptr;
        } else if(name == "ledger_linegraph_legend") {
            auto ptr = make_element_by_type<window_element_base>(state, id);
            commodity_linegraph_legend = ptr.get();
            return ptr;
        } else if(name == "ledger_linegraph_bg") {
            auto ptr = make_element_by_type<image_element_base>(state, id);
            commodity_linegraph_image = ptr.get();
            return ptr;
        } else if(name == "page_number") {
            auto ptr = make_element_by_type<simple_text_element_base>(state, id);
            page_number_text = ptr.get();
            return ptr;
        } else if(name == "ledger_header") {
            auto ptr = make_element_by_type<simple_text_element_base>(state, id);
            ledger_header_text = ptr.get();
            return ptr;
        } else if(name == "prev") {
            return make_element_by_type<ledger_prev_button>(state, id);
        } else if(name == "next") {
            return make_element_by_type<ledger_next_button>(state, id);
        } else {
            return nullptr;
        }
    }

    void on_update(sys::state& state) noexcept override {
        page_number_text->set_text(state, std::to_string(page_num.value));

        hide_sub_windows(state);
        switch(page_num.value) {
        case 1:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_rank"));
            nation_ranking_listbox->set_visible(state, true);
            break;
        case 2:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_countrycompare"));
            nation_compare_listbox->set_visible(state, true);
            break;
        case 3:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_countryparty"));
            nation_party_listbox->set_visible(state, true);
            break;
        case 4:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_countrypoliticalreforms"));
            nation_political_reforms_listbox->set_visible(state, true);
            break;
        case 5:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_countrysocialreforms"));
            nation_social_reforms_listbox->set_visible(state, true);
            break;
        case 6:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_country_pops"));
            nation_pops_listbox->set_visible(state, true);
            break;
        case 7:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_provinces"));
            provinces_listbox->set_visible(state, true);
            break;
        case 8:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_province_pops"));
            provinces_listbox->set_visible(state, true);
            break;
        case 9:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_provinceproduction"));
            provinces_production_listbox->set_visible(state, true);
            break;
        case 10:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_factoryproduction"));
            factory_production_listbox->set_visible(state, true);
            break;
        case 11:
            ledger_header_text->set_text(state, text::produce_simple_string(state, "ledger_header_goods_pricehistory"));
            commodity_linegraph->set_visible(state, true);
            commodity_linegraph_legend->set_visible(state, true);
            commodity_linegraph_image->set_visible(state, true);
            break;
        default:
            break;
        }
    }

    message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
        if(payload.holds_type<ledger_page_number>()) {
            page_num = any_cast<ledger_page_number>(payload);
            for(auto& child : children)
                child->impl_set(state, payload);
            on_update(state);
            return message_result::consumed;
        }
        return message_result::unseen;
    }
};
}
