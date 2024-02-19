/**
* @file EnemySet.h
* @brief	クラス CEnemySet の宣言ファイル
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;

class EnemySet
{
public:
	EnemySet(Stage *pParent);
	~EnemySet();
	virtual void Restore(ID2D1RenderTarget *pRT, BG *pBG);	// 共有メディアファイルの読み込み　シーン開始時などに呼び出すようにする
	virtual void Finalize();	// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする
	virtual IGameObject *GetEnemyToSet(int x);	// エネミー配置メソッド　引数の数値以下でセットデータを呼び出してエネミーを配置
protected:
	Stage *m_pParent;
	int	m_iIndex;
	static SHORT m_pSetData[];
	static size_t m_szSetDaraSize;
};

