#pragma once
#include <QWidget>
#include <QCheckBox>
#include <QSet>
#include "PlotData/PlotData.h"
#include "Scale/PlotXMap.h"
#include "Scale/PlotYMap.h"
#include "PlotDefs.h"
#include "PlotLayers/PlotLayerBase.h"
class CPlot : public QWidget
{
	Q_OBJECT

public:
	CPlot(QWidget *parent);
	~CPlot();
signals:
	void SigAxisXChanged(const CPlotXMap& cAxisX);		//	时间轴发生变化时发送信号通知主窗口
	//void SigSendCurCol(const qint64 &iCol);				//	更新序列后发送当前索引
	
private slots:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
public:
	void InitPlot(const QSet<E_DATA_TYPE>& setDataType,const ChannelParam &stParam);
	//*****************************************************
	// Method：  	UpdatePlot	
	// Purpose：	更新绘图，将更新后的图层绘制出来
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//
	//*****************************************************
	void UpdatePlot();
	//释放资源
	void Clear();
	//*****************************************************
	// Method：  	SetLayerVisible	
	// Purpose：	设置制定图层是否可见
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		eLayer			-[input]指定图层
	//		bVisible		-[input]是否可见,ture可见
	//*****************************************************
	void SetLayerVisible(const E_LAYER_INDEX &eLayer,const bool bVisible);
	//*****************************************************
	// Method：  	IsLayerVisible	
	// Purpose：	获取当前图层,是否可见
	// Access:		public
	// Returns：   	bool
	// Parameter：	
	//		eLayer			-[input]指定图层
	//*****************************************************
	bool IsLayerVisible(const E_LAYER_INDEX &eLayer);
	//DEPRECATED
	//Reason：
	//		曲线信息在ChannelConfig中已经被取，不需要额外添加曲线信息，避免重复 
	//Author：xiongz [2020/8/29]
	//void AddCurve(const LineCurveInfo &stCurveInfo);
	//void AddCurve(const QVector<LineCurveInfo >&vctCurveInfo);

	const ChannelParam & GetChannelParam() const { return m_stParam; }

	//*****************************************************
	// Method：  	AppendData	
	// Purpose：	接收数据，并显示实时曲线
	// Access：    	public	
	// Returns：   	void
	// Parameter：	
	//		eType			--[input] 数据类型
	//		vctData			--[input] 待添加的数据
	//		bIsOverView		--[input] 是否全局显示
	//*****************************************************
	void AppendData(const E_DATA_TYPE &eType,const QVector<double> &vctData, const bool &bIsOverView);
	//获取时间轴,直接获取引用，避免拷贝构造
	const CPlotXMap& GetAxisX() const { return m_AxisX; }
	//设置选段,是否进入选段模式
	void SetSegMode(const bool &bSeg);
	//设置时间标记状态 add <2020/10/17> xiongz
	void SetEventMode(const bool &bEvent);
public slots:
	//设置正在选段
	void SetSegTracking(const bool &bSegTracking);
private:
	//*****************************************************
	// Method：  	UpdateLayerPix	
	// Purpose：	更新制定的图层
	// Access：    	private
	// Returns：   	void
	// Parameter：	
	//		eLayer			-[input]图层类型
	//*****************************************************
	void UpdateLayerPix(const E_LAYER_INDEX &eLayer);
	//*****************************************************
	// Method：  	GetLayerSize	
	// Purpose：	获取指定图层的尺寸
	// Access：    	private
	// Returns：   	图层尺寸	
	// Parameter：	
	//		eLayer		-[input] 图层类型
	//*****************************************************
	 QSize GetLayerSize(const E_LAYER_INDEX &eLayer)const;
	//*****************************************************
	// Method：  	GetLayerTopLeft	
	// Purpose：	获取指定图层的的左上顶点位置，避免显示不全
	// Access：    	private
	// Returns：   	图层顶点坐标
	// Parameter：	
	//		eLayer		-[input] 图层类型
	//*****************************************************
	 QPoint GetLayerTopLeft(const E_LAYER_INDEX &eLayer)const;


private:
	QMap<E_LAYER_INDEX, CPlotLayerBase *> m_mapLayers;	//空间上各图层
	ChannelParam m_stParam;								//该导参数配置
	//坐标轴
	CPlotXMap m_AxisX;
	CPlotYMap m_AxisY;
	//曲线数据
	QMap<E_DATA_TYPE, CPlotData> m_mapData;				
	bool m_bIsSeg;										//是否进行选段
	bool m_bIsSegTracking;								//是否正在选段(进入选段模式点击按下鼠标左键)
	bool m_bIsEvent;									//是否进行事件标记
	qint64 m_iReservedWidth;							//预留窗口宽度，单位pix

};
                                                  