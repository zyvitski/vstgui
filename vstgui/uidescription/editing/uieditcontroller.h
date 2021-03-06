//-----------------------------------------------------------------------------
// VST Plug-Ins SDK
// VSTGUI: Graphical User Interface Framework not only for VST plugins
//
// Version 4.3
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// (c) 2015, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __uieditcontroller__
#define __uieditcontroller__

#include "../uidescription.h"

#if VSTGUI_LIVE_EDITING

#include "../iviewcreator.h"
#include "iaction.h"
#include "../../lib/csplitview.h"
#include "../../lib/cframe.h"

namespace VSTGUI {
class UIEditView;
class UISelection;
class UIUndoManager;
class UITemplateController;
class UIEditMenuController;
class UIGridController;
class UIZoomSettingController;
class GenericStringListDataBrowserSource;
class CCommandMenuItem;

//----------------------------------------------------------------------------------------------------
class UIEditController : public CBaseObject, public IController, public ISplitViewController, public ISplitViewSeparatorDrawer, public IActionPerformer, public IKeyboardHook
{
public:
	UIEditController (UIDescription* description);

	CView* createEditView ();
	UIEditMenuController* getMenuController () const { return menuController; }
	const std::string& getEditTemplateName () const { return editTemplateName; }
	UIAttributes* getSettings ();

	void onZoomChanged (double zoom);

	void addSelectionToCurrentView (UISelection* selection);

	static UIDescription& getEditorDescription ();
	static void setupDataSource (GenericStringListDataBrowserSource* source);
	static bool std__stringCompare (const std::string* lhs, const std::string* rhs);
	static const UTF8StringPtr kEncodeBitmapsSettingsKey;
	static const UTF8StringPtr kWriteWindowsRCFileSettingsKey;
protected:
	~UIEditController ();

	static void resetScrollViewOffsets (CViewContainer* view);

	int32_t getSplitViewIndex (CSplitView* splitView);
	void setDirty (bool state);

	virtual void valueChanged (CControl* pControl) override;
	virtual CView* createView (const UIAttributes& attributes, const IUIDescription* description) override;
	virtual CView* verifyView (CView* view, const UIAttributes& attributes, const IUIDescription* description) override;
	virtual IController* createSubController (UTF8StringPtr name, const IUIDescription* description) override;

	CMessageResult notify (CBaseObject* sender, IdStringPtr message) override;

	// ISplitViewController
	virtual bool getSplitViewSizeConstraint (int32_t index, CCoord& minSize, CCoord& maxSize, CSplitView* splitView) override;
	virtual ISplitViewSeparatorDrawer* getSplitViewSeparatorDrawer (CSplitView* splitView) override;
	virtual bool storeViewSize (int32_t index, const CCoord& size, CSplitView* splitView) override;
	virtual bool restoreViewSize (int32_t index, CCoord& size, CSplitView* splitView) override;

	// ISplitViewSeparatorDrawer
	virtual void drawSplitViewSeparator (CDrawContext* context, const CRect& size, int32_t flags, int32_t index, CSplitView* splitView) override;

	// IActionPerformer
	virtual void performAction (IAction* action) override;
	virtual void performColorChange (UTF8StringPtr colorName, const CColor& newColor, bool remove = false) override;
	virtual void performTagChange (UTF8StringPtr tagName, UTF8StringPtr tagString, bool remove = false) override;
	virtual void performBitmapChange (UTF8StringPtr bitmapName, UTF8StringPtr bitmapPath, bool remove = false) override;
	virtual void performGradientChange (UTF8StringPtr gradientName, CGradient* newGradient, bool remove = false) override;
	virtual void performFontChange (UTF8StringPtr fontName, CFontRef newFont, bool remove = false) override;
	virtual void performColorNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;
	virtual void performTagNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;
	virtual void performFontNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;
	virtual void performGradientNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;
	virtual void performBitmapNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;
	virtual void performBitmapNinePartTiledChange (UTF8StringPtr bitmapName, const CRect* offsets) override;
	virtual void performBitmapFiltersChange (UTF8StringPtr bitmapName, const std::list<SharedPointer<UIAttributes> >& filterDescription) override;
	virtual void performAlternativeFontChange (UTF8StringPtr fontName, UTF8StringPtr newAlternativeFonts) override;

	virtual void beginLiveColorChange (UTF8StringPtr colorName) override;
	virtual void performLiveColorChange (UTF8StringPtr colorName, const CColor& newColor) override;
	virtual void endLiveColorChange (UTF8StringPtr colorName) override;

	virtual void performTemplateNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;
	virtual void performCreateNewTemplate (UTF8StringPtr name, UTF8StringPtr baseViewClassName) override;
	virtual void performDeleteTemplate (UTF8StringPtr name) override;
	virtual void performDuplicateTemplate (UTF8StringPtr name, UTF8StringPtr dupName) override;

	virtual void onTemplateCreation (UTF8StringPtr name, CView* view) override;
	virtual void onTemplateNameChange (UTF8StringPtr oldName, UTF8StringPtr newName) override;

	virtual void beginGroupAction (UTF8StringPtr name) override;
	virtual void finishGroupAction () override;

	// IKeyboardHook
	virtual int32_t onKeyDown (const VstKeyCode& code, CFrame* frame) override;
	virtual int32_t onKeyUp (const VstKeyCode& code, CFrame* frame) override;

	SharedPointer<UIDescription> editDescription;
	OwningPointer<UISelection> selection;
	OwningPointer<UIUndoManager> undoManager;
	OwningPointer<UIGridController> gridController;
	UIEditView* editView;
	SharedPointer<UITemplateController> templateController;
	SharedPointer<UIEditMenuController> menuController;
	SharedPointer<UIZoomSettingController> zoomSettingController;
	SharedPointer<CControl> enableEditingControl;
	SharedPointer<CControl> notSavedControl;
	SharedPointer<CControl> tabSwitchControl;
	
	std::string editTemplateName;
	std::list<SharedPointer<CSplitView> > splitViews;
	
	bool dirty;
	
	struct Template {
		std::string name;
		SharedPointer<CView> view;

		Template (const std::string& n, CView* v) : name (n), view (v) {}
		Template (const Template& c) : name (c.name), view (c.view) {}
		bool operator==(const Template& t) { return name == t.name && view == t.view; }
		bool operator==(const std::string& n) { return name == n; }
		Template& operator=(const Template& t) { name = t.name; view = t.view; return *this; }
		Template (Template&& t) noexcept { *this = std::move (t); }
		Template& operator=(Template&& t) noexcept { name = std::move (t.name); view = std::move (t.view); return *this; }
	};
	void updateTemplate (UTF8StringPtr name);
	void updateTemplate (const std::vector<Template>::const_iterator& it);
	void onTemplatesChanged ();
	void getTemplateViews (std::list<CView*>& views) const;

	std::vector<Template> templates;
private:
	void beforeSave ();
	void onTemplateSelectionChanged ();
	CMessageResult validateMenuItem (CCommandMenuItem* item);
	CMessageResult onMenuItemSelection (CCommandMenuItem* item);
	void doCopy (bool cut = false);
	void doPaste ();
	void showTemplateSettings ();
	void showFocusSettings ();
	bool doSelectionMove (const std::string& commandName, bool useGrid) const;
	bool doSelectionSize (const std::string& commandName, bool useGrid) const;
	bool doZOrderAction (bool lower);
	void doSelectAllChildren ();
	
	void onUndoManagerChanged ();
	template<typename NameChangeAction, IViewCreator::AttrType attrType> void performNameChange (UTF8StringPtr oldName, UTF8StringPtr newName, IdStringPtr groupActionName);
};

} // namespace

#endif // VSTGUI_LIVE_EDITING

#endif // __uieditcontroller__
