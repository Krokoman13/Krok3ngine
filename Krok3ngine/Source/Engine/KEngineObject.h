#pragma once

namespace Engine {
	template <typename... Args>
	class InitializableObject {
	public:
		void Initialize(Args... args) {
			if (IsInitialized())
				throw Exception::DoubleCall("Object is initialized when it was already initialized");
			baseInitialize(args...);
			initialize(args...);
			m_initialized = true;
		};

		bool IsInitialized() { return m_initialized; }
	protected:
		virtual void baseInitialize(Args...) = 0;
		virtual void initialize(Args...) {}

	private:
		bool m_initialized = false;
	};

	class LoadableObject {
	public:
		void Load() {
			if (IsLoaded()) 
				throw Exception::DoubleCall("Object is loaded when it was already loaded");
			baseLoad();
			load();
			m_loaded = true;
		}

		bool IsLoaded() { return m_loaded; }
	protected:
		virtual void baseLoad() = 0;
		virtual void load() {}

	private:
		bool m_loaded = false;
	};

	class UpdatableObject {
	public:
		void Update() {
			baseUpdate();
			update();
		}

	protected:
		virtual void baseUpdate() = 0;
		virtual void update() {}
	};

	template <typename... Args>
	class KEngineObject : public InitializableObject<Args...>, public LoadableObject, public UpdatableObject {
	public:
		void Load() {
			if (!InitializableObject<Args...>::IsInitialized())
				throw Exception::OrderMismatch("Load function called before initialization");
			LoadableObject::Load();
		}

		void Update() {
			if (!LoadableObject::IsLoaded())
				throw Exception::OrderMismatch("Update function called before loading");
			UpdatableObject::Update();
		}
	};
}