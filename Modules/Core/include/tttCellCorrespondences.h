#ifndef TTTCELLCORRESPONDENCES_H_
#define TTTCELLCORRESPONDENCES_H_
#include <itkDataObject.h>
#include <list>
namespace ttt{

	template<class TCell> class Mitosis{
	public:
		Mitosis(const TCell & cell0, const TCell & cell1a, const TCell & cell1b){

		}
	};

	template<class TCell> class Succesor{
	public:
		Succesor(const TCell & cell0, const TCell & cell1){

		}
	};

	template<class TCell> class Apoptosis{
	public:
		Apoptosis(const TCell & cell){

		}

	};

	template<class TCell> class Entering{
	public:
		Entering(const TCell & cell){

		}

	};

	template<class TCell> class Leaving{
	public:
		Leaving(const TCell & cell){

		}
	};

	template<class TTissueDescriptor> class CellCorrespondences : public itk::DataObject{

	public:
		typedef CellCorrespondences Self;
		typedef itk::DataObject Superclass;
		typedef itk::SmartPointer<Self> Pointer;
		typedef itk::SmartPointer<const Self> ConstPointer;

		typedef typename TTissueDescriptor::CellGraphType::CellVertexHandler CellType;

		typedef std::pair<CellType,std::pair<CellType,CellType> > MitosisType;
		typedef std::pair<CellType,CellType> SuccesorType;
		typedef CellType LeavingType;
		typedef CellType EnteringType;
		typedef CellType ApoptosisType;

		itkNewMacro(Self);
		itkTypeMacro(CellCorrespondences,itk::DataObject);

		typedef std::set<SuccesorType> SuccesorContainer;
		typedef std::set<EnteringType> EnteringContainer;
		typedef std::set<LeavingType> LeavingContainer;
		typedef std::set<MitosisType > MitosisContainer;
		typedef std::set<ApoptosisType > ApoptosisContainer;

		typedef typename SuccesorContainer::iterator SuccesorIterator;
		typedef typename EnteringContainer::iterator EnteringIterator;
		typedef typename LeavingContainer::iterator LeavingIterator;
		typedef typename MitosisContainer::iterator MitosisIterator;
		typedef typename ApoptosisContainer::iterator ApoptosisIterator;


		itkGetObjectMacro(Tissue0,TTissueDescriptor)
		itkSetObjectMacro(Tissue0,TTissueDescriptor)

		itkGetObjectMacro(Tissue1,TTissueDescriptor)
		itkSetObjectMacro(Tissue1,TTissueDescriptor)



		void AddSuccesor(const SuccesorType & succesor ){
			m_SuccesorCells.insert(succesor);
		}
		void AddEntering(const EnteringType & entering){
			m_EnteringCells.insert(entering);
		}
		void AddLeaving(const LeavingType & leaving){
			m_LeavingCells.insert(leaving);
		}
		void AddMitosis(const MitosisType & mitosis){
			m_MitosisCells.insert(mitosis);
		}
		void AddApoptosis(const ApoptosisType & apoptosis){
			m_ApoptosisCells.insert(apoptosis);
		}

		void RemoveSuccesor(const SuccesorType & succesor){
			m_SuccesorCells.remove(succesor);
		}
		void RemoveEntering(const EnteringType & entering){
			m_EnteringCells.remove(entering);
		}
		void RemoveLeaving(const LeavingType & leaving){
			m_LeavingCells.remove(leaving);
		}
		void RemoveMitosis(const MitosisType & mitosis){
			m_MitosisCells.remove(mitosis);
		}
		void RemoveApoptosis(const ApoptosisType & apoptosis){
			m_ApoptosisCells.remove(apoptosis);
		}

		SuccesorIterator SuccesorBegin(){
			return m_SuccesorCells.begin();
		}
		SuccesorIterator SuccesorEnd(){
			return m_SuccesorCells.end();
		}
		EnteringIterator EnteringBegin(){
			return m_EnteringCells.begin();
		}
		EnteringIterator EnteringEnd(){
			return m_EnteringCells.end();

		}

		LeavingIterator LeavingBegin(){
			return m_LeavingCells.begin();
		}
		LeavingIterator LeavingEnd(){
			return m_LeavingCells.end();
		}
		MitosisIterator MitosisBegin(){
			return m_MitosisCells.begin();
		}
		MitosisIterator MitosisEnd(){
			return m_MitosisCells.end();
		}

		ApoptosisIterator ApoptosisBegin(){
			return m_ApoptosisCells.begin();
		}
		ApoptosisIterator ApoptosisEnd(){
			return m_ApoptosisCells.end();
		}


		bool IsSuccesor(const CellType & cell){

		}

		bool IsEntering(const CellType & cell){

		}

		bool IsLeaving(const CellType & cell){

		}
		bool IsMitosis(const CellType & cell){

		}
		bool IsApoptosis(const CellType & cell){

		}


	private:
		CellCorrespondences(){

		}

		typename TTissueDescriptor::Pointer m_Tissue0;
		typename TTissueDescriptor::Pointer m_Tissue1;

		LeavingContainer m_LeavingCells;
		ApoptosisContainer m_ApoptosisCells;
		MitosisContainer m_MitosisCells;
		EnteringContainer m_EnteringCells;
		SuccesorContainer m_SuccesorCells;

	};
}

#endif
