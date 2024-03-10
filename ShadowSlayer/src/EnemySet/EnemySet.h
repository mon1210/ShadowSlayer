/**
* @file  EnemySet.h
* @brief クラス EnemySet の宣言ファイル
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;

class EnemySet
{
public:
	/**
	* @brief EnemySetのコンストラクタ
	*/
	EnemySet(Stage *pParent);

	/**
	* @brief EnemySetのデストラクタ
	*/
	~EnemySet();

	/**
	* @brief	共有メディアファイルの読み込み
	* @note		シーン開始時などに呼び出すようにする
	*/
	void Restore(ID2D1RenderTarget *pRT, BG *pBG);

	/**
	* @brief	共有メディアファイルの消去
	* @note		シーン削除時などに呼び出すようにする
	*/
	void Finalize();

	/*
	* @brief エネミー配置メソッド
	* @note  引数の数値以下でセットデータを呼び出してエネミーを配置
	*/
	IGameObject *GetEnemyToSet(int x);
protected:
	Stage *m_pParent;
	int	m_iIndex;
	static SHORT m_pSetData[];
	static size_t m_szSetDaraSize;
};

