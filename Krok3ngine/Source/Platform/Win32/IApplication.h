#pragma once

#define ENTRYAPP(x) IApplication* EntryApplication() { return new x; }

class KROK3NGINE_API IApplication {
public:
	IApplication();
	virtual ~IApplication() {};

public:
	virtual VOID SetupPerGameSettings() = 0;
	virtual VOID Initialize() = 0;
	virtual VOID Update() = 0;
};

IApplication* EntryApplication();