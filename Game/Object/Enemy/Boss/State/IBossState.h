#pragma once
//前方宣言
class Boss;
class IBossState
{
public:
	IBossState();
	~IBossState();

	virtual void Init(Boss* boss);
	virtual void Update(Boss* boss);

private:

};
