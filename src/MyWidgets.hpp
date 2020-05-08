#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>

#include <cwctype>				// iswdigit

namespace MyWidgets
{
	struct Button : public nana::button
	{
	public:
		template<typename ...Args>
		Button(Args&& ...args) : nana::button{ std::forward<Args>(args)... }
		{
			this->enable_focus_color(false);
			this->edge_effects(false);
		}
	};
	

	struct NumTextbox : public nana::textbox
	{
	private:
		const size_t max_chars = 30;

		bool num_filter(wchar_t ch)
		{
			if (iswdigit(ch) && this->caption().length() < max_chars
				||L'-' == ch
				|| ch == nana::keyboard::backspace || ch == nana::keyboard::del
				|| ch == nana::keyboard::paste
				|| ch == nana::keyboard::copy)
				return true;
			else
				return false;
		}

		bool text_filter()
		{
			return this->caption().length() < max_chars;
		}

	public:
		template<typename ...Args>
		NumTextbox(Args&& ...args) : nana::textbox{ std::forward<Args>(args)... }
		{
			configure();
		}

		void configure()
		{
			this->text_align(nana::align::right);
			this->multi_lines(false);
			nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
			this->set_accept([&](wchar_t ch)->bool { return num_filter(ch); });			
		}

		void text_mode()
		{
			this->set_accept([&](wchar_t ch)->bool { return text_filter(); });
		}
	};


	struct Listbox : public nana::listbox
	{
	public:
		template<typename ...Args>
		Listbox(Args&& ...args) : nana::listbox{ std::forward<Args>(args)... }
		{
			this->sortable(false);
			this->column_movable(false);
			this->column_resizable(false);
			this->is_single_enabled(false);
		}
	};
}
