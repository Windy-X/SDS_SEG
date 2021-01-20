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
	void SigAxisXChanged(const CPlotXMap& cAxisX);		//	ʱ���ᷢ���仯ʱ�����ź�֪ͨ������
	//void SigSendCurCol(const qint64 &iCol);				//	�������к��͵�ǰ����
	
private slots:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
public:
	void InitPlot(const QSet<E_DATA_TYPE>& setDataType,const ChannelParam &stParam);
	//*****************************************************
	// Method��  	UpdatePlot	
	// Purpose��	���»�ͼ�������º��ͼ����Ƴ���
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//
	//*****************************************************
	void UpdatePlot();
	//�ͷ���Դ
	void Clear();
	//*****************************************************
	// Method��  	SetLayerVisible	
	// Purpose��	�����ƶ�ͼ���Ƿ�ɼ�
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		eLayer			-[input]ָ��ͼ��
	//		bVisible		-[input]�Ƿ�ɼ�,ture�ɼ�
	//*****************************************************
	void SetLayerVisible(const E_LAYER_INDEX &eLayer,const bool bVisible);
	//*****************************************************
	// Method��  	IsLayerVisible	
	// Purpose��	��ȡ��ǰͼ��,�Ƿ�ɼ�
	// Access:		public
	// Returns��   	bool
	// Parameter��	
	//		eLayer			-[input]ָ��ͼ��
	//*****************************************************
	bool IsLayerVisible(const E_LAYER_INDEX &eLayer);
	//DEPRECATED
	//Reason��
	//		������Ϣ��ChannelConfig���Ѿ���ȡ������Ҫ�������������Ϣ�������ظ� 
	//Author��xiongz [2020/8/29]
	//void AddCurve(const LineCurveInfo &stCurveInfo);
	//void AddCurve(const QVector<LineCurveInfo >&vctCurveInfo);

	const ChannelParam & GetChannelParam() const { return m_stParam; }

	//*****************************************************
	// Method��  	AppendData	
	// Purpose��	�������ݣ�����ʾʵʱ����
	// Access��    	public	
	// Returns��   	void
	// Parameter��	
	//		eType			--[input] ��������
	//		vctData			--[input] ����ӵ�����
	//		bIsOverView		--[input] �Ƿ�ȫ����ʾ
	//*****************************************************
	void AppendData(const E_DATA_TYPE &eType,const QVector<double> &vctData, const bool &bIsOverView);
	//��ȡʱ����,ֱ�ӻ�ȡ���ã����⿽������
	const CPlotXMap& GetAxisX() const { return m_AxisX; }
	//����ѡ��,�Ƿ����ѡ��ģʽ
	void SetSegMode(const bool &bSeg);
	//����ʱ����״̬ add <2020/10/17> xiongz
	void SetEventMode(const bool &bEvent);
public slots:
	//��������ѡ��
	void SetSegTracking(const bool &bSegTracking);
private:
	//*****************************************************
	// Method��  	UpdateLayerPix	
	// Purpose��	�����ƶ���ͼ��
	// Access��    	private
	// Returns��   	void
	// Parameter��	
	//		eLayer			-[input]ͼ������
	//*****************************************************
	void UpdateLayerPix(const E_LAYER_INDEX &eLayer);
	//*****************************************************
	// Method��  	GetLayerSize	
	// Purpose��	��ȡָ��ͼ��ĳߴ�
	// Access��    	private
	// Returns��   	ͼ��ߴ�	
	// Parameter��	
	//		eLayer		-[input] ͼ������
	//*****************************************************
	 QSize GetLayerSize(const E_LAYER_INDEX &eLayer)const;
	//*****************************************************
	// Method��  	GetLayerTopLeft	
	// Purpose��	��ȡָ��ͼ��ĵ����϶���λ�ã�������ʾ��ȫ
	// Access��    	private
	// Returns��   	ͼ�㶥������
	// Parameter��	
	//		eLayer		-[input] ͼ������
	//*****************************************************
	 QPoint GetLayerTopLeft(const E_LAYER_INDEX &eLayer)const;


private:
	QMap<E_LAYER_INDEX, CPlotLayerBase *> m_mapLayers;	//�ռ��ϸ�ͼ��
	ChannelParam m_stParam;								//�õ���������
	//������
	CPlotXMap m_AxisX;
	CPlotYMap m_AxisY;
	//��������
	QMap<E_DATA_TYPE, CPlotData> m_mapData;				
	bool m_bIsSeg;										//�Ƿ����ѡ��
	bool m_bIsSegTracking;								//�Ƿ�����ѡ��(����ѡ��ģʽ�������������)
	bool m_bIsEvent;									//�Ƿ�����¼����
	qint64 m_iReservedWidth;							//Ԥ�����ڿ�ȣ���λpix

};
                                                  