#pragma once

#include "gui_element_types.hpp"
#include "gui_graphics.hpp"
#include "gui_common_elements.hpp"
#include "province.hpp"
#include "color.hpp"
#include "triggers.hpp"

namespace ui {

const std::vector<dcon::pop_id>& get_pop_window_list(sys::state& state);

class pop_culture_text : public generic_settable_element<simple_text_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept {
		const auto fat_id = dcon::fatten(state.world, content);
		const auto cfat_id = dcon::fatten(state.world, fat_id.get_culture());
		set_text(state, text::produce_simple_string(state, cfat_id.get_name()));
	}
};
class pop_size_text : public generic_settable_element<simple_text_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept {
		const auto fat_id = dcon::fatten(state.world, content);
		set_text(state, std::to_string(int32_t(fat_id.get_size())));
	}
};
class pop_location_text : public generic_settable_element<simple_text_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept {
		auto fat_id = dcon::fatten(state.world, content);
		auto lcfat_id = dcon::fatten(state.world, fat_id.get_pop_location()).get_province();
		set_text(state, text::produce_simple_string(state, lcfat_id.get_name()));
	}
};
class pop_militancy_text : public generic_settable_element<simple_text_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept {
		const auto fat_id = dcon::fatten(state.world, content);
		set_text(state, text::format_float(fat_id.get_militancy()));
	}
};
class pop_con_text : public generic_settable_element<simple_text_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept {
		const auto fat_id = dcon::fatten(state.world, content);
		set_text(state, text::format_float(fat_id.get_consciousness()));
	}
};
class pop_literacy_text : public generic_settable_element<simple_text_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept {
		const auto fat_id = dcon::fatten(state.world, content);
		set_text(state, text::format_percentage(fat_id.get_literacy(), 2));
	}
};

enum class pop_list_sort : uint8_t {
	size,
	type,
	nationality,
	religion,
	location,
	mil,
	con,
	ideology,
	issues,
	unemployment,
	cash,
	subsistence,
	eve,
	luxury,
	revoltrisk,
	change,
	literacy
};

class standard_pop_progress_bar : public generic_settable_element<progress_bar, dcon::pop_id> {
public:
    virtual float get_progress(sys::state& state) noexcept {
        return 0.f;
    }

    void on_update(sys::state& state) noexcept override {
        progress = get_progress(state);
    }

    void on_create(sys::state& state) noexcept override {
        base_data.position.x -= 14;
        base_data.position.y -= 12;
        base_data.size.y = 25;
        base_data.size.x = 13;
    }
};
class standard_pop_needs_progress_bar : public generic_settable_element<progress_bar, dcon::pop_id> {
public:
    virtual float get_progress(sys::state& state) noexcept {
        return 0.f;
    }

    void on_update(sys::state& state) noexcept override {
        progress = get_progress(state);
    }

    void on_create(sys::state& state) noexcept override {
        base_data.position.x -= 15;
        base_data.position.y -= 4;
        base_data.size.y = 20;
        base_data.size.x = 15;
    }
};

class pop_unemployment_progress_bar : public standard_pop_progress_bar {
public:
	float get_progress(sys::state& state) noexcept override {
		auto pfat_id = dcon::fatten(state.world, content);
		if(state.world.pop_type_get_has_unemployment(state.world.pop_get_poptype(content)))
			return (1 - pfat_id.get_employment() / pfat_id.get_size());
		return 0.0f;
	}

	tooltip_behavior has_tooltip(sys::state& state) noexcept override {
		return tooltip_behavior::variable_tooltip;
	}

	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override {
		auto box = text::open_layout_box(contents, 0);

		auto pfat_id = dcon::fatten(state.world, content);
		float un_empl = state.world.pop_type_get_has_unemployment(state.world.pop_get_poptype(content)) ? (1 - pfat_id.get_employment() / pfat_id.get_size()) : 0.0f;
		text::add_to_layout_box(contents, state, box, text::produce_simple_string(state, "unemployment"));
		text::add_space_to_layout_box(contents, state, box);
		text::add_to_layout_box(contents, state, box, text::fp_percentage{ un_empl });
		text::close_layout_box(contents, box);
	}
};
class pop_life_needs_progress_bar : public standard_pop_needs_progress_bar {
public:
	float get_progress(sys::state& state) noexcept override {
		auto fat_id = dcon::fatten(state.world, content);
		return fat_id.get_life_needs_satisfaction();
	}

	tooltip_behavior has_tooltip(sys::state& state) noexcept override {
		return tooltip_behavior::variable_tooltip;
	}

	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override {
		auto box = text::open_layout_box(contents, 0);

		auto fat_id = dcon::fatten(state.world, content);
		text::add_to_layout_box(contents, state, box, text::produce_simple_string(state, "life_needs"));
		text::add_space_to_layout_box(contents, state, box);
		text::add_to_layout_box(contents, state, box, text::fp_percentage{ fat_id.get_life_needs_satisfaction() });
		text::close_layout_box(contents, box);
	}
};

class pop_everyday_needs_progress_bar : public standard_pop_needs_progress_bar {
public:
	float get_progress(sys::state& state) noexcept override {
		auto fat_id = dcon::fatten(state.world, content);
		return fat_id.get_everyday_needs_satisfaction();
	}

	tooltip_behavior has_tooltip(sys::state& state) noexcept override {
		return tooltip_behavior::variable_tooltip;
	}

	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override {
		auto box = text::open_layout_box(contents, 0);

		auto fat_id = dcon::fatten(state.world, content);
		text::add_to_layout_box(contents, state, box, text::produce_simple_string(state, "everyday_needs"));
		text::add_space_to_layout_box(contents, state, box);
		text::add_to_layout_box(contents, state, box, text::fp_percentage{ fat_id.get_everyday_needs_satisfaction() });
		text::close_layout_box(contents, box);
	}
};

class pop_luxury_needs_progress_bar : public standard_pop_needs_progress_bar {
public:
	float get_progress(sys::state& state) noexcept override {
		auto fat_id = dcon::fatten(state.world, content);
		return fat_id.get_luxury_needs_satisfaction();
	}

	tooltip_behavior has_tooltip(sys::state& state) noexcept override {
		return tooltip_behavior::variable_tooltip;
	}

	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override {
		auto box = text::open_layout_box(contents, 0);

		auto fat_id = dcon::fatten(state.world, content);
		text::add_to_layout_box(contents, state, box, text::produce_simple_string(state, "luxury_needs"));
		text::add_space_to_layout_box(contents, state, box);
		text::add_to_layout_box(contents, state, box, text::fp_percentage{ fat_id.get_luxury_needs_satisfaction() });
		text::close_layout_box(contents, box);
	}
};

class pop_issues_piechart : public demographic_piechart<dcon::pop_id, dcon::issue_option_id> {
protected:
	void for_each_demo(sys::state& state, std::function<void(dcon::issue_option_id)> fun) override {
		state.world.for_each_issue_option(fun);
	}
public:
	void on_create(sys::state &state) noexcept override{
		base_data.size.x = 28;
		base_data.size.y = 28;
		base_data.position.x -= 13;
		radius = 13;
	}
};
class pop_ideology_piechart : public demographic_piechart<dcon::pop_id, dcon::ideology_id> {
protected:
	void for_each_demo(sys::state& state, std::function<void(dcon::ideology_id)> fun) override {
		state.world.for_each_ideology(fun);
	}
public:
	void on_create(sys::state &state) noexcept override{
		base_data.size.x = 28;
		base_data.size.y = 28;
		base_data.position.x -= 13;
		radius = 13;
	}
};

typedef std::variant<
	std::monostate,
	dcon::pop_id
> pop_details_data;
class pop_details_promotion_percent_text : public simple_text_element_base {
	dcon::value_modifier_key mod_key{};
	dcon::pop_location_id pop_loc{};
	float chance = 0.f;
public:
	void on_update(sys::state& state) noexcept override {
		set_text(state, text::format_percentage(chance, 1));
	}

	tooltip_behavior has_tooltip(sys::state& state) noexcept override {
		return tooltip_behavior::variable_tooltip;
	}

	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override {
		// TODO: value_modifier_key effect description tooltip here
		//auto prov_id = state.world.pop_location_get_province(pop_loc);
		//auto nat_id = state.world.province_get_nation_from_province_ownership(prov_id);
		//trigger_description(state, contents, mod_key, trigger::to_generic(prov_id), trigger::to_generic(nat_id), 0);
	}

	message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<dcon::pop_location_id>()) {
			pop_loc = any_cast<dcon::pop_location_id>(payload);
			on_update(state);
			return message_result::consumed;
		} else if(payload.holds_type<dcon::value_modifier_key>()) {
			mod_key = any_cast<dcon::value_modifier_key>(payload);
			on_update(state);
			return message_result::consumed;
		} else if(payload.holds_type<float>()) {
			chance = any_cast<float>(payload);
			on_update(state);
			return message_result::consumed;
		}
		return message_result::unseen;
	}
};
template<size_t N>
class pop_details_promotion_window : public window_element_base {
	dcon::pop_type_id content{};
	float chance = 0.f;

	pop_type_icon* type_icon = nullptr;
	simple_text_element_base* percent_text = nullptr;
public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "pop_type") {
			auto ptr = make_element_by_type<pop_type_icon>(state, id);
			type_icon = ptr.get();
			return ptr;
		} else if(name == "percentage") {
			auto ptr = make_element_by_type<pop_details_promotion_percent_text>(state, id);
			percent_text = ptr.get();
			return ptr;
		} else {
			return nullptr;
		}
	}

	void on_update(sys::state& state) noexcept override {
		Cyto::Any payload = content;
		type_icon->impl_set(state, payload);
	}

	message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<dcon::pop_type_id>()) {
			content = any_cast<dcon::pop_type_id>(payload);
			on_update(state);
			return message_result::consumed;
		} else if(payload.holds_type<dcon::pop_location_id>() || payload.holds_type<dcon::value_modifier_key>() || payload.holds_type<float>()) {
			percent_text->impl_set(state, payload);
			return message_result::consumed;
		}
		return message_result::unseen;
	}
};
class pop_details_window : public generic_settable_element<window_element_base, pop_details_data> {
	pop_type_icon* type_icon = nullptr;
	religion_type_icon* religion_icon = nullptr;
	simple_text_element_base* savings_text = nullptr;
	std::vector<element_base*> promotion_windows;

	template<std::size_t... Targs>
	void generate_promotion_items(sys::state& state, std::integer_sequence<std::size_t, Targs...> const &) {
		const xy_pair cell_offset{ 312, 153 };
		(([&]{
			auto win = make_element_by_type<pop_details_promotion_window<Targs>>(state, state.ui_state.defs_by_name.find("pop_promotion_item")->second.definition);
			win->base_data.position.x = cell_offset.x + (Targs * win->base_data.size.x);
			win->base_data.position.y = cell_offset.y;
			promotion_windows.push_back(win.get());
			add_child_to_front(std::move(win));
		})(), ...);
	}
public:
	void on_create(sys::state& state) noexcept override {
		window_element_base::on_create(state);
		set_visible(state, false);
		generate_promotion_items(state, std::integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5, 6>{});
	}

	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "close_button") {
			return make_element_by_type<generic_close_button>(state, id);
		} else if(name == "pop_type_icon") {
			auto ptr = make_element_by_type<pop_type_icon>(state, id);
			type_icon = ptr.get();
			return ptr;
		} else if(name == "pop_size") {
			return make_element_by_type<pop_size_text>(state, id);
		} else if(name == "pop_culture") {
			return make_element_by_type<pop_culture_text>(state, id);
		} else if(name == "pop_location") {
			return make_element_by_type<pop_location_text>(state, id);
		} else if(name == "mil_value") {
			return make_element_by_type<pop_militancy_text>(state, id);
		} else if(name == "con_value") {
			return make_element_by_type<pop_con_text>(state, id);
		} else if(name == "literacy_value") {
			return make_element_by_type<pop_literacy_text>(state, id);
		} else if(name == "icon_religion") {
			auto ptr = make_element_by_type<religion_type_icon>(state, id);
			religion_icon = ptr.get();
			return ptr;
		} else if(name == "bank_value") {
			auto ptr = make_element_by_type<simple_text_element_base>(state, id);
			savings_text = ptr.get();
			return ptr;
		} else if(name == "pop_unemployment_bar") {
			return make_element_by_type<pop_unemployment_progress_bar>(state, id);
		} else if(name == "lifeneed_progress") {
			return make_element_by_type<pop_life_needs_progress_bar>(state, id);
		} else if(name == "eveneed_progress") {
			return make_element_by_type<pop_everyday_needs_progress_bar>(state, id);
		} else if(name == "luxneed_progress") {
			return make_element_by_type<pop_luxury_needs_progress_bar>(state, id);
		} else {
			return nullptr;
		}
	}

	void on_update(sys::state& state) noexcept override {
		if(!std::holds_alternative<dcon::pop_id>(content))
			return;
		
		auto fat_id = dcon::fatten(state.world, std::get<dcon::pop_id>(content));
		Cyto::Any tpayload = fat_id.get_poptype().id;
		type_icon->impl_set(state, tpayload);
		// updated below ...
		Cyto::Any rpayload = fat_id.get_religion().id;
		religion_icon->impl_set(state, rpayload);
		// updated below ...
		savings_text->set_text(state, text::format_float(state.world.pop_get_savings(fat_id.id)));
		Cyto::Any payload = fat_id.id;
		for(auto& c : children) {
			c->impl_set(state, payload);
			c->impl_on_update(state);
		}

		// Hide all promotion windows...
		for(std::size_t i = 0; i < promotion_windows.size(); ++i)
			promotion_windows[i]->set_visible(state, false);
		std::unordered_map<dcon::pop_type_id::value_base_t, float> distrib = {};
		auto total = 0.f;
		state.world.for_each_pop_type([&](dcon::pop_type_id ptid){
			auto mod_key = fat_id.get_poptype().get_promotion(ptid);
			auto prov_id = state.world.pop_location_get_province(state.world.pop_get_pop_location_as_pop(fat_id.id));
			auto nat_id = state.world.province_get_nation_from_province_ownership(prov_id);
			auto chance = trigger::evaluate_additive_modifier(state, mod_key, trigger::to_generic(prov_id), trigger::to_generic(nat_id), 0);
			distrib[dcon::pop_type_id::value_base_t(ptid.index())] = chance;
			total += chance;
		});
		// And then show them as appropriate!
		size_t index = 0;
		for(const auto& e : distrib)
			if(e.second > 0.f && index < promotion_windows.size()) {
				promotion_windows[index]->set_visible(state, true);
				Cyto::Any pt_payload = dcon::pop_type_id(e.first);
				promotion_windows[index]->impl_set(state, pt_payload);
				Cyto::Any pl_payload = state.world.pop_get_pop_location_as_pop(fat_id.id);
				promotion_windows[index]->impl_set(state, pl_payload);
				Cyto::Any chance_payload = float(e.second / total);
				promotion_windows[index]->impl_set(state, chance_payload);
				++index;
			}
	}
};

class pop_details_icon : public generic_settable_element<button_element_base, dcon::pop_id> {
public:
	void on_update(sys::state& state) noexcept override {
		auto fat_id = dcon::fatten(state.world, state.world.pop_get_poptype(content));
		frame = int32_t(fat_id.get_sprite() - 1);
	}

	void button_action(sys::state& state) noexcept override {
		Cyto::Any payload = pop_details_data(content);
		state.ui_state.population_subwindow->impl_set(state, payload);
	}

	tooltip_behavior has_tooltip(sys::state& state) noexcept override {
		return tooltip_behavior::variable_tooltip;
	}

	void update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept override {
		auto name = state.world.pop_type_get_name(state.world.pop_get_poptype(content));
		if(bool(name)) {
			auto box = text::open_layout_box(contents, 0);
			text::add_to_layout_box(contents, state, box, name);
			text::close_layout_box(contents, box);
		}
	}
};
class pop_item : public listbox_row_element_base<dcon::pop_id> {
private:
	pop_details_icon* type = nullptr;
	image_element_base* religion = nullptr;
	simple_text_element_base* cash = nullptr;
public:
	void on_create(sys::state& state) noexcept override {
		listbox_row_element_base<dcon::pop_id>::on_create(state);
	}

	std::unique_ptr<element_base>
		make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "pop_size") {
			return make_element_by_type<pop_size_text>(state, id);
		} else if(name == "pop_nation") {
			return make_element_by_type<pop_culture_text>(state, id);
		} else if(name == "pop_location") {
			return make_element_by_type<pop_location_text>(state, id);
		} else if(name == "pop_mil") {
			return make_element_by_type<pop_militancy_text>(state, id);
		} else if(name == "pop_con") {
			return make_element_by_type<pop_con_text>(state, id);
		} else if(name == "pop_type") {
			auto ptr = make_element_by_type<pop_details_icon>(state, id);
			type = ptr.get();
			return ptr;
		} else if(name == "pop_religion") {
			auto ptr = make_element_by_type<religion_type_icon>(state, id);
			religion = ptr.get();
			return ptr;
		} else if(name == "pop_ideology") {
			return make_element_by_type<pop_ideology_piechart>(state, id);
		} else if(name == "pop_issues") {
			return make_element_by_type<pop_issues_piechart>(state, id);
		} else if(name == "pop_unemployment_bar") {
			return make_element_by_type<pop_unemployment_progress_bar>(state, id);
		} else if(name == "pop_cash") {
			auto ptr = make_element_by_type<simple_text_element_base>(state, id);
			cash = ptr.get();
			return ptr;
		} else if(name == "lifeneed_progress") {
			return make_element_by_type<pop_life_needs_progress_bar>(state, id);
		} else if(name == "eveneed_progress") {
			return make_element_by_type<pop_everyday_needs_progress_bar>(state, id);
		} else if(name == "luxneed_progress") {
			return make_element_by_type<pop_luxury_needs_progress_bar>(state, id);
		} else if(name == "pop_literacy") {
			return make_element_by_type<pop_literacy_text>(state, id);
		} else {
			return nullptr;
		}
	};

	void update(sys::state& state) noexcept override {
		auto fat_id = dcon::fatten(state.world, content);

		Cyto::Any tpayload = fat_id.get_poptype().id;
		type->impl_set(state, tpayload);
		// updated below...
		Cyto::Any rpayload = fat_id.get_religion().id;
		religion->impl_set(state, rpayload);
		// updated below...
		cash->set_text(state, text::format_float(state.world.pop_get_savings(fat_id.id)));
		Cyto::Any payload = content;
		for(auto& c : children) {
			c->impl_set(state, payload);
			c->impl_on_update(state);
		}
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<dcon::pop_id>()) {
			payload.emplace<dcon::pop_id>(content);
			return message_result::consumed;
		}
		return listbox_row_element_base<dcon::pop_id>::get(state, payload);
	}
};
class pop_listbox : public listbox_element_base<pop_item, dcon::pop_id> {
protected:
	std::string_view get_row_element_name() override {
		return "popinfomember_popview";
	}
};

typedef std::variant<
	std::monostate,
	dcon::nation_id,
	dcon::state_instance_id,
	dcon::province_id
> pop_left_side_data;
template<typename T>
class pop_left_side_button : public button_element_base {
public:
	void on_update(sys::state& state) noexcept override {
		if(parent) {
			Cyto::Any payload = T{};
			parent->impl_get(state, payload);
			T id = any_cast<T>(payload);
			if(state.ui_state.population_subwindow) {
				Cyto::Any filter_payload = pop_list_filter{};
				state.ui_state.population_subwindow->impl_get(state, filter_payload);
				auto filter = any_cast<pop_list_filter>(filter_payload);
				frame = std::holds_alternative<T>(filter) && std::get<T>(filter) == id
					? 1 : 0;
			}
		}
	}
	void button_action(sys::state& state) noexcept override {
		if(parent) {
			Cyto::Any payload = T{};
			parent->impl_get(state, payload);
			T id = any_cast<T>(payload);
			if(state.ui_state.population_subwindow) {
				Cyto::Any new_payload = pop_list_filter(id);
				state.ui_state.population_subwindow->impl_set(state, new_payload);
			}
			on_update(state);
		}
	}
};
class pop_left_side_country_window : public generic_settable_element<window_element_base, dcon::nation_id> {
public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "poplistbutton") {
			return make_element_by_type<pop_left_side_button<dcon::nation_id>>(state, id);
		} else if(name == "poplist_name") {
			return make_element_by_type<generic_name_text<dcon::nation_id>>(state, id);
		} else if(name == "poplist_numpops") {
			return make_element_by_type<nation_population_text>(state, id);
		} else if(name == "growth_indicator") {
			return make_element_by_type<image_element_base>(state, id);
		} else {
			return nullptr;
		}
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<dcon::nation_id>()) {
			payload.emplace<dcon::nation_id>(content);
			return message_result::consumed;
		}
		return window_element_base::get(state, payload);
	}
};
typedef std::variant<
	std::monostate,
	dcon::state_instance_id,
	bool
> pop_left_side_expand_action;
class pop_left_side_expand_button : public button_element_base {
public:
	void on_create(sys::state& state) noexcept override {
		button_element_base::on_create(state);
		set_button_text(state, "");
	}

	void on_update(sys::state& state) noexcept override {
		if(parent) {
			Cyto::Any id_payload = dcon::state_instance_id{};
			parent->impl_get(state, id_payload);
			auto id = any_cast<dcon::state_instance_id>(id_payload);

			Cyto::Any payload = pop_left_side_expand_action(id);
			parent->impl_get(state, payload);
			frame = std::get<bool>(any_cast<pop_left_side_expand_action>(payload)) ? 1 : 0;
		}
	}

	void button_action(sys::state& state) noexcept override {
		if(parent) {
			Cyto::Any payload = dcon::state_instance_id{};
			parent->impl_get(state, payload);
			auto id = any_cast<dcon::state_instance_id>(payload);
			if(state.ui_state.population_subwindow) {
				Cyto::Any new_payload = pop_left_side_expand_action(id);
				state.ui_state.population_subwindow->impl_set(state, new_payload);
			}
			on_update(state);
		}
	}
};
class pop_left_side_state_window : public generic_settable_element<window_element_base, dcon::state_instance_id> {
	image_element_base* colonial_icon = nullptr;
public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "poplistbutton") {
			return make_element_by_type<pop_left_side_button<dcon::state_instance_id>>(state, id);
		} else if(name == "poplist_name") {
			return make_element_by_type<state_name_text>(state, id);
		} else if(name == "poplist_numpops") {
			return make_element_by_type<state_population_text>(state, id);
		} else if(name == "colonial_state_icon") {
			auto ptr = make_element_by_type<image_element_base>(state, id);
			colonial_icon = ptr.get();
			return ptr;
		} else if(name == "state_focus") {
			return make_element_by_type<button_element_base>(state, id);
		} else if(name == "expand") {
			return make_element_by_type<pop_left_side_expand_button>(state, id);
		} else if(name == "growth_indicator") {
			return make_element_by_type<image_element_base>(state, id);
		} else {
			return nullptr;
		}
	}

	void on_update(sys::state& state) noexcept override {
		colonial_icon->set_visible(state, state.world.province_get_is_colonial(state.world.state_instance_get_capital(content)));
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<dcon::state_instance_id>()) {
			payload.emplace<dcon::state_instance_id>(content);
			return message_result::consumed;
		}
		return window_element_base::get(state, payload);
	}
};
class pop_left_side_province_window : public generic_settable_element<window_element_base, dcon::province_id> {
public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "poplistbutton") {
			return make_element_by_type<pop_left_side_button<dcon::province_id>>(state, id);
		} else if(name == "poplist_name") {
			return make_element_by_type<generic_name_text<dcon::province_id>>(state, id);
		} else if(name == "poplist_numpops") {
			return make_element_by_type<province_population_text>(state, id);
		} else if(name == "growth_indicator") {
			return make_element_by_type<image_element_base>(state, id);
		} else {
			return nullptr;
		}
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<dcon::province_id>()) {
			payload.emplace<dcon::province_id>(content);
			return message_result::consumed;
		}
		return window_element_base::get(state, payload);
	}
};

class pop_left_side_item : public listbox_row_element_base<pop_left_side_data> {
	pop_left_side_country_window* country_window = nullptr;
	pop_left_side_state_window* state_window = nullptr;
	pop_left_side_province_window* province_window = nullptr;
public:
	void on_create(sys::state& state) noexcept override {
		listbox_row_element_base<pop_left_side_data>::on_create(state);

		auto ptr1 = make_element_by_type<pop_left_side_country_window>(state, state.ui_state.defs_by_name.find("poplistitem_country")->second.definition);
		country_window = ptr1.get();
		add_child_to_back(std::move(ptr1));

		auto ptr2 = make_element_by_type<pop_left_side_state_window>(state, state.ui_state.defs_by_name.find("poplistitem_state")->second.definition);
		state_window = ptr2.get();
		add_child_to_back(std::move(ptr2));

		auto ptr3 = make_element_by_type<pop_left_side_province_window>(state, state.ui_state.defs_by_name.find("poplistitem_province")->second.definition);
		province_window = ptr3.get();
		add_child_to_back(std::move(ptr3));
		// After this, the widget will be immediately set by the parent
	}

	void update(sys::state& state) noexcept override {
		country_window->set_visible(state, std::holds_alternative<dcon::nation_id>(content));
		state_window->set_visible(state, std::holds_alternative<dcon::state_instance_id>(content));
		province_window->set_visible(state, std::holds_alternative<dcon::province_id>(content));
		if (std::holds_alternative<dcon::nation_id>(content)) {
			Cyto::Any new_payload = std::get<dcon::nation_id>(content);
			country_window->impl_set(state, new_payload);
		} else if (std::holds_alternative<dcon::state_instance_id>(content)) {
			Cyto::Any new_payload = std::get<dcon::state_instance_id>(content);
			state_window->impl_set(state, new_payload);
		} else if (std::holds_alternative<dcon::province_id>(content)) {
			Cyto::Any new_payload = std::get<dcon::province_id>(content);
			province_window->impl_set(state, new_payload);
		}
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<pop_left_side_data>()) {
			payload.emplace<pop_left_side_data>(content);
			return message_result::consumed;
		}
		return listbox_row_element_base<pop_left_side_data>::get(state, payload);
	}
};

class pop_left_side_listbox : public listbox_element_base<pop_left_side_item, pop_left_side_data> {
protected:
	std::string_view get_row_element_name() override {
		return "pop_left_side_list_base_window";
	}
};

template<typename T>
class pop_distrobution_plupp : public tinted_image_element_base {
	T content{};
public:
	uint32_t get_tint_color(sys::state& state) noexcept override {
		return ogl::get_ui_color<T>(state, content);
	}
	
	message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<T>()) {
			content = any_cast<T>(payload);
			on_update(state);
			return message_result::consumed;
		}
		return message_result::unseen;
	}
};

template<typename T>
class pop_distrobution_piechart : public piechart<T> {
protected:
	std::unordered_map<typename T::value_base_t, float> get_distribution(sys::state& state) noexcept override {
		std::unordered_map<typename T::value_base_t, float> distrib{};
		if(piechart<T>::parent) {
			auto& pop_list = get_pop_window_list(state);

			auto total = 0.f;
			for(const auto pop_id : pop_list) {
				const auto weight_fn = [&](auto id) {
					auto weight = state.world.pop_get_demographics(pop_id, pop_demographics::to_key(state, id));
					distrib[typename T::value_base_t(id.index())] += weight;
					total += weight;
				};
				// Can obtain via simple pop_demographics query
				if constexpr(std::is_same_v<T, dcon::issue_option_id>)
					state.world.for_each_issue_option(weight_fn);
				else if constexpr(std::is_same_v<T, dcon::ideology_id>)
					state.world.for_each_ideology(weight_fn);
				// Needs to be queried directly from the pop
				if constexpr(std::is_same_v<T, dcon::culture_id>) {
					distrib[typename T::value_base_t(state.world.pop_get_culture(pop_id).id.index())] += state.world.pop_get_size(pop_id);
					total += state.world.pop_get_size(pop_id);
				} else if constexpr(std::is_same_v<T, dcon::religion_id>) {
					distrib[typename T::value_base_t(state.world.pop_get_religion(pop_id).id.index())] += state.world.pop_get_size(pop_id);
					total += state.world.pop_get_size(pop_id);
				} else if constexpr(std::is_same_v<T, dcon::pop_type_id>) {
					distrib[typename T::value_base_t(state.world.pop_get_poptype(pop_id).id.index())] += state.world.pop_get_size(pop_id);
					total += state.world.pop_get_size(pop_id);
				} else if constexpr(std::is_same_v<T, dcon::political_party_id>) {
					auto prov_id = state.world.pop_location_get_province(state.world.pop_get_pop_location_as_pop(pop_id));
					if(state.world.province_get_is_colonial(prov_id))
						continue;
					auto tag = state.world.nation_get_identity_from_identity_holder(state.world.province_get_nation_from_province_ownership(prov_id));
					auto start = state.world.national_identity_get_political_party_first(tag).id.index();
					auto end = start + state.world.national_identity_get_political_party_count(tag);
					for(int32_t i = start; i < end; i++) {
						auto pid = T(typename T::value_base_t(i));
						if(politics::political_party_is_active(state, pid)) {
							auto support = politics::party_total_support(state, pop_id, pid, state.world.province_get_nation_from_province_ownership(prov_id), prov_id);
							distrib[typename T::value_base_t(pid.index())] += support;
							total += support;
						}
					}
				}
			}
			for(auto& e : distrib)
				if(e.second > 0.f)
					e.second /= total;
		}
		return distrib;
	}
public:
    void on_create(sys::state &state) noexcept override {
		piechart<T>::on_create(state);
		//piechart<T>::base_data.position.x -= piechart<T>::base_data.size.x;
		piechart<T>::base_data.position.x -= 17;
		piechart<T>::radius = float(piechart<T>::base_data.size.x);
		piechart<T>::base_data.size.x *= 2;
		piechart<T>::base_data.size.y *= 2;
		piechart<T>::on_update(state);
    }
};
template<typename T>
class pop_distrobution_item : public listbox_row_element_base<T> {
	element_base* title_text = nullptr;
	simple_text_element_base* value_text = nullptr;
public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "legend_color") {
			return make_element_by_type<pop_distrobution_plupp<T>>(state, id);
		} else if(name == "legend_title") {
			return make_element_by_type<generic_name_text<T>>(state, id);
		} else if(name == "legend_value") {
			auto ptr = make_element_by_type<simple_text_element_base>(state, id);
			value_text = ptr.get();
			return ptr;
		} else {
			return nullptr;
		}
	}
	void update(sys::state& state) noexcept override {
		T content = listbox_row_element_base<T>::content;
		for(auto& c : listbox_row_element_base<T>::children) {
			Cyto::Any payload = content;
			c->impl_set(state, payload);
		}
		if(listbox_row_element_base<T>::parent) {
			auto& pop_list = get_pop_window_list(state);

			auto value = 0.f;
			auto total = 0.f;
			for(const auto pop_id : pop_list) {
				const auto weight_fn = [&](auto id) {
					auto weight = state.world.pop_get_demographics(pop_id, pop_demographics::to_key(state, id));
					if(id == content)
						value += weight;
					total += weight;
				};
				// Obtained with simple query
				if constexpr(std::is_same_v<T, dcon::issue_option_id>)
					state.world.for_each_issue_option(weight_fn);
				else if constexpr(std::is_same_v<T, dcon::ideology_id>)
					state.world.for_each_ideology(weight_fn);
				// Needs to be queried directly from the pop
				const auto direct_weight_fn = [&](auto&& fn, auto id){
					const auto sub_id = fn(id).id;
					const auto size = state.world.pop_get_size(id);
					if(sub_id == content)
						value += size;
					total += size;
				};
				if constexpr(std::is_same_v<T, dcon::culture_id>) {
					const auto sub_id = state.world.pop_get_culture(pop_id).id;
					const auto size = state.world.pop_get_size(pop_id);
					if(sub_id == content)
						value += size;
					total += size;	
				} else if constexpr(std::is_same_v<T, dcon::religion_id>) {
					const auto sub_id = state.world.pop_get_religion(pop_id).id;
					const auto size = state.world.pop_get_size(pop_id);
					if(sub_id == content)
						value += size;
					total += size;	
				} else if constexpr(std::is_same_v<T, dcon::pop_type_id>) {
					const auto sub_id = state.world.pop_get_poptype(pop_id).id;
					const auto size = state.world.pop_get_size(pop_id);
					if(sub_id == content)
						value += size;
					total += size;	
				} else if constexpr(std::is_same_v<T, dcon::political_party_id>) {
					auto prov_id = state.world.pop_location_get_province(state.world.pop_get_pop_location_as_pop(pop_id));
					if(state.world.province_get_is_colonial(prov_id))
						continue;
					auto tag = state.world.nation_get_identity_from_identity_holder(state.world.province_get_nation_from_province_ownership(prov_id));
					auto start = state.world.national_identity_get_political_party_first(tag).id.index();
					auto end = start + state.world.national_identity_get_political_party_count(tag);
					for(int32_t i = start; i < end; i++) {
						auto pid = T(typename T::value_base_t(i));
						if(politics::political_party_is_active(state, pid)) {
							auto support = politics::party_total_support(state, pop_id, pid, state.world.province_get_nation_from_province_ownership(prov_id), prov_id);
							if(pid == content)
								value += support;
							total += support;
						}
					}
				}
			}
			value_text->set_text(state, text::format_percentage(total > 0.f && value > 0.f ? value / total : 0.f, 1));
		}
	}
};
template<typename T>
class pop_distrobution_listbox : public listbox_element_base<pop_distrobution_item<T>, T> {
public:
	std::string_view get_row_element_name() override {
		return "pop_legend_item";
	}
};
template<typename T>
class pop_distrobution_window : public window_element_base {
	pop_distrobution_listbox<T>* distrib_listbox;
public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "item_name") {
			auto ptr = make_element_by_type<simple_text_element_base>(state, id);
			if constexpr(std::is_same_v<T, dcon::issue_option_id>)
				ptr->set_text(state, text::produce_simple_string(state, "dominant_issues_disttitle"));
			else if constexpr(std::is_same_v<T, dcon::culture_id>)
				ptr->set_text(state, text::produce_simple_string(state, "nationality_disttitle"));
			else if constexpr(std::is_same_v<T, dcon::ideology_id>)
				ptr->set_text(state, text::produce_simple_string(state, "ideology_disttitle"));
			else if constexpr(std::is_same_v<T, dcon::religion_id>)
				ptr->set_text(state, text::produce_simple_string(state, "religion_disttitle"));
			else if constexpr(std::is_same_v<T, dcon::pop_type_id>)
				ptr->set_text(state, text::produce_simple_string(state, "workforce_disttitle"));
			else if constexpr(std::is_same_v<T, dcon::political_party_id>)
				ptr->set_text(state, text::produce_simple_string(state, "electorate_disttitle"));
			return ptr;
		} else if(name == "chart") {
			return make_element_by_type<pop_distrobution_piechart<T>>(state, id);
		} else if(name == "member_names") {
			auto ptr = make_element_by_type<pop_distrobution_listbox<T>>(state, id);
			distrib_listbox = ptr.get();
			return ptr;
		} else {
			return nullptr;
		}
	}
	void on_update(sys::state& state) noexcept override {
		if(parent) {
			auto& pop_list = get_pop_window_list(state);
			
			std::unordered_map<typename T::value_base_t, float> distrib{};
			for(const auto pop_id : pop_list) {
				const auto weight_fn = [&](auto id) {
					auto weight = state.world.pop_get_demographics(pop_id, pop_demographics::to_key(state, id));
					distrib[typename T::value_base_t(id.index())] += weight;
				};
				// Can obtain via simple pop_demographics query
				if constexpr(std::is_same_v<T, dcon::issue_option_id>)
					state.world.for_each_issue_option(weight_fn);
				else if constexpr(std::is_same_v<T, dcon::ideology_id>)
					state.world.for_each_ideology(weight_fn);
				// Needs to be queried directly from the pop
				if constexpr(std::is_same_v<T, dcon::culture_id>)
					distrib[typename T::value_base_t(state.world.pop_get_culture(pop_id).id.index())] += state.world.pop_get_size(pop_id);
				else if constexpr(std::is_same_v<T, dcon::religion_id>)
					distrib[typename T::value_base_t(state.world.pop_get_religion(pop_id).id.index())] += state.world.pop_get_size(pop_id);
				else if constexpr(std::is_same_v<T, dcon::pop_type_id>)
					distrib[typename T::value_base_t(state.world.pop_get_poptype(pop_id).id.index())] += state.world.pop_get_size(pop_id);
				else if constexpr(std::is_same_v<T, dcon::political_party_id>) {
					auto prov_id = state.world.pop_location_get_province(state.world.pop_get_pop_location_as_pop(pop_id));
					if(state.world.province_get_is_colonial(prov_id))
						continue;
					auto tag = state.world.nation_get_identity_from_identity_holder(state.world.province_get_nation_from_province_ownership(prov_id));
					auto start = state.world.national_identity_get_political_party_first(tag).id.index();
					auto end = start + state.world.national_identity_get_political_party_count(tag);
					for(int32_t i = start; i < end; i++) {
						auto pid = T(typename T::value_base_t(i));
						if(politics::political_party_is_active(state, pid)) {
							auto support = politics::party_total_support(state, pop_id, pid, state.world.province_get_nation_from_province_ownership(prov_id), prov_id);
							distrib[typename T::value_base_t(pid.index())] += support;
						}
					}
				}
			}
			std::vector<std::pair<T, float>> sorted_distrib{};
			for(const auto& e : distrib)
				if(e.second > 0.f)
					sorted_distrib.push_back(std::make_pair(T(e.first), e.second));
			std::sort(sorted_distrib.begin(), sorted_distrib.end(), [&](auto a, auto b) {
				return a.second > b.second;
			});
			distrib_listbox->row_contents.clear();
			for(const auto& e : sorted_distrib)
				distrib_listbox->row_contents.push_back(e.first);
			distrib_listbox->update(state);
		}
	}
};

typedef std::variant<
	std::monostate,
	dcon::pop_type_id,
	bool
> pop_filter_data;
class pop_filter_button : public generic_settable_element<button_element_base, dcon::pop_type_id> {
public:
	void on_update(sys::state& state) noexcept override {
		frame = int32_t(state.world.pop_type_get_sprite(content) - 1);
	}

	void button_action(sys::state& state) noexcept override {
		if(parent) {
			Cyto::Any payload = pop_filter_data(content);
			parent->impl_set(state, payload);
		}
	}
};

class population_window : public window_element_base {
private:
	pop_listbox* country_pop_listbox = nullptr;
	pop_left_side_listbox* left_side_listbox = nullptr;
	pop_list_filter filter = std::monostate{};
	pop_details_window* details_win = nullptr;
	std::vector<element_base*> dist_windows;
	// Whetever or not to show provinces below the state element in the listbox!
	ankerl::unordered_dense::map<dcon::state_instance_id::value_base_t, bool> view_expanded_state;
	std::vector<bool> pop_filters;
	std::vector<dcon::pop_id> shared_pop_list;

	void update_shared_pop_list(sys::state& state) {
		shared_pop_list.clear();

		auto nation_id = std::holds_alternative<dcon::nation_id>(filter)
			? std::get<dcon::nation_id>(filter)
			: state.local_player_nation;
		std::vector<dcon::state_instance_id> state_list{};
		for(auto si : state.world.nation_get_state_ownership(nation_id))
			state_list.push_back(si.get_state().id);
		
		std::vector<dcon::province_id> province_list{};
		for(auto& state_id : state_list) {
			if(std::holds_alternative<dcon::state_instance_id>(filter)
			&& std::get<dcon::state_instance_id>(filter) != state_id)
				continue;
			auto fat_id = dcon::fatten(state.world, state_id);
			province::for_each_province_in_state_instance(state, fat_id, [&](dcon::province_id id) {
				province_list.push_back(id);
			});
		}

		for(auto& province_id : province_list) {
			if(std::holds_alternative<dcon::province_id>(filter)
			&& std::get<dcon::province_id>(filter) != province_id)
				continue;
			auto fat_id = dcon::fatten(state.world, province_id);
			fat_id.for_each_pop_location_as_province([&](dcon::pop_location_id id) {
				// Apply pop filters properly...
				auto pop_id = state.world.pop_location_get_pop(id);
				auto pt_id = state.world.pop_get_poptype(pop_id);
				if(pop_filters[dcon::pop_type_id::value_base_t(pt_id.id.index())])
					shared_pop_list.push_back(pop_id);
			});
		}
	}

	void sort_pop_list(sys::state& state) {
		std::sort(country_pop_listbox->row_contents.begin(), country_pop_listbox->row_contents.end(), [&](auto a, auto b) {
			auto a_fat_id = dcon::fatten(state.world, a);
			auto b_fat_id = dcon::fatten(state.world, b);
			return a_fat_id.get_size() > b_fat_id.get_size();
		});
	}

	void populate_left_side_list(sys::state& state) {
		auto nation_id = std::holds_alternative<dcon::nation_id>(filter)
			? std::get<dcon::nation_id>(filter)
			: state.local_player_nation;

		// & then populate the separate, left side listbox
		left_side_listbox->row_contents.push_back(pop_left_side_data(nation_id));

		// States are sorted by total population
		std::vector<dcon::state_instance_id> state_list;
		for(auto si : state.world.nation_get_state_ownership(nation_id))
			state_list.push_back(si.get_state().id);
		std::sort(state_list.begin(), state_list.end(), [&](auto a, auto b) {
			// Colonial states go last
			if(state.world.province_get_is_colonial(state.world.state_instance_get_capital(a)) != state.world.province_get_is_colonial(state.world.state_instance_get_capital(b)))
				return !state.world.province_get_is_colonial(state.world.state_instance_get_capital(a));
			return state.world.state_instance_get_demographics(a, demographics::total) > state.world.state_instance_get_demographics(b, demographics::total);
		});

		std::vector<dcon::province_id> province_list;
		for(const auto state_id : state_list) {
			left_side_listbox->row_contents.push_back(pop_left_side_data(state_id));
			// Provinces are sorted by total population too
			province_list.clear();
			auto fat_id = dcon::fatten(state.world, state_id);
			province::for_each_province_in_state_instance(state, fat_id, [&](dcon::province_id id) {
				province_list.push_back(id);
			});
			std::sort(province_list.begin(), province_list.end(), [&](auto a, auto b) {
				return state.world.province_get_demographics(a, demographics::total) > state.world.province_get_demographics(b, demographics::total);
			});
			// Only put if the state is "expanded"
			if(view_expanded_state[dcon::state_instance_id::value_base_t(state_id.index())] == true)
				for(const auto province_id : province_list)
					left_side_listbox->row_contents.push_back(pop_left_side_data(province_id));
		}
	}

	template<typename T, typename... Targs>
	void generate_distrobution_windows(sys::state& state) {
		auto win = make_element_by_type<T>(state, state.ui_state.defs_by_name.find("distribution_window")->second.definition);
		dist_windows.push_back(win.get());
		add_child_to_front(std::move(win));

		if constexpr(sizeof...(Targs))
			generate_distrobution_windows<Targs...>(state);
	}
public:
	void on_create(sys::state& state) noexcept override {
		window_element_base::on_create(state);
		set_visible(state, false);
		
		{
			generate_distrobution_windows<
				pop_distrobution_window<dcon::pop_type_id>,
				pop_distrobution_window<dcon::religion_id>,
				pop_distrobution_window<dcon::ideology_id>,
				pop_distrobution_window<dcon::culture_id>,
				pop_distrobution_window<dcon::issue_option_id>,
				pop_distrobution_window<dcon::political_party_id>
			>(state);

			// It should be proper to reposition the windows now
			const xy_pair cell_offset = state.ui_defs.gui[state.ui_state.defs_by_name.find("popdistribution_start")->second.definition].position;
			const xy_pair cell_size = state.ui_defs.gui[state.ui_state.defs_by_name.find("popdistribution_offset")->second.definition].position;
			xy_pair offset = cell_offset;
			for(const auto win : dist_windows) {
				win->base_data.position = offset;
				offset.x += cell_size.x;
				if(offset.x + cell_size.x >= base_data.size.x) {
					offset.x = cell_offset.x;
					offset.y += cell_size.y;
				}
			}
		}

		{
			// Now add the filtering windows
			const xy_pair cell_offset = state.ui_defs.gui[state.ui_state.defs_by_name.find("popfilter_start")->second.definition].position;
			const xy_pair cell_size = state.ui_defs.gui[state.ui_state.defs_by_name.find("popfilter_offset")->second.definition].position;
			xy_pair offset = cell_offset;

			state.world.for_each_pop_type([&](dcon::pop_type_id id) {
				auto win = make_element_by_type<pop_filter_button>(state, state.ui_state.defs_by_name.find("pop_filter_button")->second.definition);
				Cyto::Any payload = id;
				win->base_data.position = offset;
				win->impl_set(state, payload);
				add_child_to_front(std::move(win));
				offset.x += cell_size.x;
				pop_filters.push_back(true);
			});
		}

		auto win7 = make_element_by_type<pop_details_window>(state, state.ui_state.defs_by_name.find("pop_details_win")->second.definition);
		details_win = win7.get();
		add_child_to_front(std::move(win7));
	}

	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "close_button") {
			return make_element_by_type<generic_close_button>(state, id);
		} else if(name == "pop_list") {
			auto ptr = make_element_by_type<pop_listbox>(state, id);
			country_pop_listbox = ptr.get();
			return ptr;
		} else if(name == "external_scroll_slider") {
			auto ptr = make_element_by_type<opaque_element_base>(state, id);
			return ptr;
		} else if(name == "sortby_size_button") {
			auto ptr = make_element_by_type<button_element_base>(state, id);
			return ptr;
		} else if(name == "sortby_type_button") {
			auto ptr = make_element_by_type<button_element_base>(state, id);
			return ptr;
		} else if(name == "sortby_nationality_button") {
			auto ptr = make_element_by_type<button_element_base>(state, id);
			return ptr;
		} else if(name == "sortby_religion_button") {
			auto ptr = make_element_by_type<button_element_base>(state, id);
			return ptr;
		} else if(name == "sortby_location_button") {
			auto ptr = make_element_by_type<button_element_base>(state, id);
			return ptr;
		} else if(name == "sortby_mil_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_con_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_ideology_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_issues_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_unemployment_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_cash_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_subsistence_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_eve_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_luxury_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_revoltrisk_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_change_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "sortby_literacy_button") {
			auto ptr = make_element_by_type<generic_opaque_checkbox_button<dcon::nation_id>>(state, id);
			ptr->base_data.position.y -= 1; // Nudge
			return ptr;
		} else if(name == "pop_province_list") {
			auto ptr = make_element_by_type<pop_left_side_listbox>(state, id);
			left_side_listbox = ptr.get();
			return ptr;
		} else {
			return nullptr;
		}
	}

	void on_update(sys::state& state) noexcept override {
		update_shared_pop_list(state);
		if(country_pop_listbox) {
			country_pop_listbox->row_contents.clear();
			country_pop_listbox->row_contents = get_pop_window_list(state);
			sort_pop_list(state);
			country_pop_listbox->update(state);
		}
		if(left_side_listbox) {
			left_side_listbox->row_contents.clear();
			populate_left_side_list(state);
			left_side_listbox->update(state);
		}
		for(auto& e : dist_windows)
			e->impl_on_update(state);
		window_element_base::on_update(state);
	}

	message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<pop_list_filter>()) {
			filter = any_cast<pop_list_filter>(payload);
			on_update(state);
			return message_result::consumed;
		} else if(payload.holds_type<pop_details_data>()) {
			move_child_to_front(details_win);
			details_win->set_visible(state, true);
			details_win->impl_set(state, payload);
			return message_result::consumed;
		} else if(payload.holds_type<pop_left_side_expand_action>()) {
			auto expand_action = any_cast<pop_left_side_expand_action>(payload);
			auto sid = std::get<dcon::state_instance_id>(expand_action);
			view_expanded_state[dcon::state_instance_id::value_base_t(sid.index())] = !view_expanded_state[dcon::state_instance_id::value_base_t(sid.index())];
			on_update(state);
			return message_result::consumed;
		} else if(payload.holds_type<pop_filter_data>()) {
			auto data = any_cast<pop_filter_data>(payload);
			auto ptid = std::get<dcon::pop_type_id>(data);
			pop_filters[dcon::pop_type_id::value_base_t(ptid.index())] = !pop_filters[dcon::pop_type_id::value_base_t(ptid.index())];
			on_update(state);
			return message_result::consumed;
		}
		return message_result::unseen;
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<pop_list_filter>()) {
			payload.emplace<pop_list_filter>(filter);
			return message_result::consumed;
		} else if(payload.holds_type<pop_left_side_expand_action>()) {
			auto expand_action = any_cast<pop_left_side_expand_action>(payload);
			auto sid = std::get<dcon::state_instance_id>(expand_action);
			payload.emplace<pop_left_side_expand_action>(pop_left_side_expand_action(view_expanded_state[dcon::state_instance_id::value_base_t(sid.index())]));
			return message_result::consumed;
		} else if(payload.holds_type<pop_filter_data>()) {
			auto data = any_cast<pop_filter_data>(payload);
			auto ptid = std::get<dcon::pop_type_id>(data);
			payload.emplace<pop_filter_data>(pop_filter_data(pop_filters[dcon::pop_type_id::value_base_t(ptid.index())]));
			return message_result::consumed;
		}
		return message_result::unseen;
	}

	friend const std::vector<dcon::pop_id>& get_pop_window_list(sys::state& state);
};

const std::vector<dcon::pop_id>& get_pop_window_list(sys::state& state) {
	static const std::vector<dcon::pop_id> empty{};
	if(state.ui_state.population_subwindow)
		return static_cast<population_window*>(state.ui_state.population_subwindow)->shared_pop_list;
	return empty;
}
}
