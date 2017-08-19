#pragma once

struct Wallet
{
	int		money;

	//Market�֘A
	int		price;	//Market����U�荞�ݎ��ɉ��i����������
	int		expenses;	//�x�o���L�^
	int		income;		//�������L�^

	Wallet();
	void	sendTo(Wallet* _wallet, int	_money);	//����
	void	add(int _money);
	void	pull(int _money);
	int		id() const;
};

extern Array<Wallet>	wallets;

int	getNewWalletID();