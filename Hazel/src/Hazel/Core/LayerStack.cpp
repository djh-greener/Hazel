#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {
	 LayerStack::LayerStack()
	{
	}

	 LayerStack::~LayerStack()
	 {
		 for (auto Layer : m_Layers)
		 {
			 Layer->OnDetach();
			 delete Layer;
		 }
	 }

	void  LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin()+ m_LayerInsert, layer);
		m_LayerInsert++;
		layer->OnAttach();
	}

	void  LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void  LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin()+m_LayerInsert, layer);
		if (it != m_Layers.begin() + m_LayerInsert)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void  LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsert, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);

		}
	}
}