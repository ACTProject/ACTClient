#pragma once


class Client : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

};
