


--获取普通的窗口事件,不必写Window窗口的,Window窗口的在C++里面已经支持
function getCommonWindowEventList()


end

--以下是某种特定类型窗口的List,根据windowType不同,可以执行不同的添加

function getSpecialWindowEventList(windowType)
	--ActionButton
	if windowType == "Falagard/ActionButton" then
		getPushButtonEventList()
		getActionButtonEventList()
	elseif windowType == "Falagard/MeshWindow" then
		getMeshWindowEventList()
	elseif windowType == "Falagard/SoftKeyWindow" then
		getSoftKeyBoardEventList()
	elseif windowType == "Falagard/SceneMap" then
		getSceneMapBoardEventList()
	elseif windowType == "Falagard/Animate" then
		getPushButtonEventList()
	elseif windowType == "Falagard/CheckBox" then
		getPushButtonEventList()
	elseif windowType == "Falagard/SystemButton" then
		getPushButtonEventList()
	elseif windowType == "Falagard/RewardItem" then
		getPushButtonEventList()
	elseif windowType == "Falagard/SuperProgress" then
		getSuperProgressEventList()
	elseif windowType == "Falagard/IMEEditBox" then
		getEditboxEventList()
	elseif windowType == "Falagard/MultiIMEEditBox" then
		getMutiEditBoxEventList()
	elseif windowType == "Falagard/Tree" then
		getTreeEventList()
	elseif windowType == "Falagard/Button" then
		getPushButtonEventList()
	elseif windowType == "Falagard/Combobox" then
		getComboBoxEventList()
	elseif windowType == "Falagard/Editbox" then
		getEditboxEventList()
	elseif windowType == "Falagard/MultiLineEditbox" then
		getEditboxEventList()
		getMutiEditBoxEventList()
	elseif windowType == "Falagard/FrameWindow" then
		getFrameWindowEventList()
	elseif windowType == "Falagard/Listbox" then
		getListBoxEventList()
	elseif windowType == "Falagard/ListHeader" then
		getListHeaderEventList()
	elseif windowType == "Falagard/ListHeaderSegment" then
		getListHeaderSegementListEvent()
	elseif windowType == "Falagard/Menubar" then
		getMenuBaseEventList()
	elseif windowType == "Falagard/MultiColumnList" then
		getColumnListEventList()
	elseif windowType == "Falagard/PopupMenu" then
		getMenuBaseEventList()
	elseif windowType == "Falagard/ProgressBar" then
		getSuperProgressEventList()
	elseif windowType == "Falagard/RadioButton" then
		getRadioButtonEventList()
	elseif windowType == "Falagard/ScrollablePane" then
		getScrollablePanelEventList()
	elseif windowType == "Falagard/Scrollbar" then
		getScrollbarEventList()
	elseif windowType == "Falagard/Slider" then
		getSliderEventList()
	elseif windowType == "Falagard/Spinner" then
		getSpinnerEventList()
	elseif windowType == "Falagard/TabControl" then
		getTabControlEventList()
	elseif windowType == "Falagard/Thumb" then
		getThumbEventList()
	elseif windowType == "Falagard/Tooltip" then
		getTooltipEventList()
	else
		System:print("缺少扩展事件:"..windowType .."\n")
	end
end

--Falagard/ActionButton
function getActionButtonEventList()
    System:addEventToList("DragStarted");
    System:addEventToList("DragEnded");
    System:addEventToList("DragPositionChanged");
    System:addEventToList("DragEnabledChanged");
    System:addEventToList("DragThresholdChanged");
    System:addEventToList("DragDropTargetChanged");
    System:addEventToList("DropItemEnters");
    System:addEventToList("DropItemLeaves");
    System:addEventToList("DropItemDropped");
		System:addEventToList("RBClicked");
end

--Falagard/MeshWindow
function getMeshWindowEventList()
    System:addEventToList("MeshHook");
   	System:addEventToList("MeshShown");
    System:addEventToList("MeshHidden");
end

--Falagard/SoftKeyWindow
function getSoftKeyBoardEventList()
	System:addEventToList("ReturnPush");
end

--Falagard/SceneMap
function getSceneMapBoardEventList()
	System:addEventToList("MouseRButtonDownCtrl");
end

--all public from ButtonBase
function getPushButtonEventList()
	System:addEventToList("Clicked");
end

--Falagard/SuperProgress
function getSuperProgressEventList()
	System:addEventToList("ProgressChanged");
	System:addEventToList("ProgressDone");
end

--all enditbox event
function getEditboxEventList()
	System:addEventToList("ReadOnlyChanged");
	System:addEventToList("MaskRenderChanged" );
	System:addEventToList("MaskCPChanged" );
	System:addEventToList("ValidatorChanged" );
	System:addEventToList("MaxTextLenChanged" );
	System:addEventToList("TextInvalidated" );
	System:addEventToList("InvalidInputAttempt" );
	System:addEventToList("TextCaratMoved" );
	System:addEventToList("TextSelectChanged" );
	System:addEventToList("EditboxFull" );
	System:addEventToList("TextAccepted" );
	System:addEventToList("TabPressed" );
end

-- muti editbox event
function getMutiEditBoxEventList()
	System:addEventToList("ReadOnlyChanged" );
	System:addEventToList("WordWrapModeChanged" );
	System:addEventToList("MaximumTextLengthChanged" );
	System:addEventToList("CaratMoved" );
	System:addEventToList("TextSelectionChanged" );
	System:addEventToList("EditboxFullEvent" );
	System:addEventToList("VertScrollbarModeChanged" );
	System:addEventToList("HorzScrollbarModeChanged" );
end
--all tree event
function getTreeEventList()
	System:addEventToList("ListItemsChanged" );
	System:addEventToList("ItemSelectionChanged" );
	System:addEventToList("SortModeChanged" );
	System:addEventToList("MuliselectModeChanged" );
	System:addEventToList("VertScrollModeChanged" );
	System:addEventToList("HorzScrollModeChanged" );
	System:addEventToList("BranchOpened" );
	System:addEventToList("BranchClosed" );
end

--Combobox BaseEvent
function getComboBoxEventList()
	System:addEventToList("ReadOnlyChanged" );
	System:addEventToList("ValidationStringChanged" );
	System:addEventToList("MaximumTextLengthChanged" );
	System:addEventToList("TextInvalidatedEvent" );
	System:addEventToList("InvalidEntryAttempted" );
	System:addEventToList("CaratMoved" );
	System:addEventToList("TextSelectionChanged" );
	System:addEventToList("EditboxFullEvent" );
	System:addEventToList("TextAcceptedEvent" );
	
	-- event names from list box
	System:addEventToList("ListContentsChanged" );
	System:addEventToList("ListSelectionChanged" );
	System:addEventToList("SortModeChanged" );
	System:addEventToList("VertScrollbarModeChanged" );
	System:addEventToList("HorzScrollbarModeChanged" );
	
	-- events we produce / generate ourselves
	System:addEventToList("DropListDisplayed" );
	System:addEventToList("DropListRemoved" );
	System:addEventToList("ListSelectionAccepted" );
end

--FrameWindow Base Event
function getFrameWindowEventList()
	System:addEventToList("RollupToggled" );
	System:addEventToList("CloseClicked" );
end

function getListBoxEventList()
	System:addEventToList("ListItemsChanged" );
	System:addEventToList("ItemSelectionChanged" );
	System:addEventToList("SortModeChanged" );
	System:addEventToList("MuliselectModeChanged" );
	System:addEventToList("VertScrollModeChanged" );
	System:addEventToList("HorzScrollModeChanged" );
end

function getListHeaderEventList()
	System:addEventToList("SortColumnChanged" );
	System:addEventToList("SortDirectionChanged" );
	System:addEventToList("SegmentSized" );
	System:addEventToList("SegmentClicked" );
	System:addEventToList("SplitterDoubleClicked" );
	System:addEventToList("SegmentSequenceChanged" );
	System:addEventToList("SegmentAdded" );
	System:addEventToList("SegmentRemoved" );
	System:addEventToList("SortSettingChanged" );
	System:addEventToList("DragMoveSettingChanged" );
	System:addEventToList("DragSizeSettingChanged" );
	System:addEventToList("SegmentOffsetChanged" );
end

function getListHeaderSegementListEvent()
	System:addEventToList("SegmentClicked" );
	System:addEventToList("SplitterDoubleClicked" );
	System:addEventToList("SizingSettingChanged" );
	System:addEventToList("SortDirectionChanged" );
	System:addEventToList("MovableSettingChanged" );
	System:addEventToList("SegmentDragStart" );
	System:addEventToList("SegmentDragStop" );
	System:addEventToList("SegmentDragPositionChanged" );
	System:addEventToList("SegmentSized" );
	System:addEventToList("ClickableSettingChanged" );
end

function getMenuBaseEventList()
	System:addEventToList("PopupOpened" );
	System:addEventToList("PopupClosed" );
end

function getColumnListEventList()
	System:addEventToList("SelectModeChanged" );
	System:addEventToList("NomSelColChanged" );
	System:addEventToList("NomSelRowChanged" );
	System:addEventToList("VertBarModeChanged" );
	System:addEventToList("HorzBarModeChanged" );
	System:addEventToList("SelectionChanged" );
	System:addEventToList("ContentsChanged" );
	System:addEventToList("SortColChanged" );
	System:addEventToList("SortDirChanged" );
	System:addEventToList("ColSized" );
	System:addEventToList("ColMoved" );
end

function getRadioButtonEventList()
	System:addEventToList("SelectStateChanged" );
end

function getScrollablePanelEventList()
	System:addEventToList("ContentPaneChanged");
  System:addEventToList("VertScrollbarModeChanged");
  System:addEventToList("HorzScrollbarModeChanged");
  System:addEventToList("AutoSizeSettingChanged");
  System:addEventToList("ContentPaneScrolled");
end

function getScrollbarEventList()
	System:addEventToList("ScrollPosChanged" );
	System:addEventToList("ThumbTrackStarted" );
	System:addEventToList("ThumbTrackEnded" );
	System:addEventToList("ScrollConfigChanged" );
end

function getSliderEventList()
	System:addEventToList("ValueChanged" );
	System:addEventToList("ThumbTrackStarted" );
	System:addEventToList("ThumbTrackEnded" );
end

function getSpinnerEventList()
  System:addEventToList("ValueChanged");
  System:addEventToList("StepChanged");
  System:addEventToList("MaximumValueChanged");
  System:addEventToList("MinimumValueChanged");
  System:addEventToList("TextInputModeChanged");
end

function getTabControlEventList()
	System:addEventToList("TabSelectionChanged" );
end

function getThumbEventList()
	System:addEventToList("ThumbPosChanged" );
	System:addEventToList("ThumbTrackStarted" );
	System:addEventToList("ThumbTrackEnded" );
end

function getTooltipEventList()
  System:addEventToList("HoverTimeChanged");
  System:addEventToList("DisplayTimeChanged");
  System:addEventToList("FadeTimeChanged");
  System:addEventToList("TooltipActive");
  System:addEventToList("TooltipInactive");
end


--获取lua的关键词和常量
function getLuaKeywordAndConst()
	local szkeyword = "and break do else  elseif end function if in local nil not or repeat return then until while this";
	local szConst = "abs acos appendto ascii asin assert atan atan2 call ceil clock collectgarbage copytagmethods cos date deg dofile dostring error execute exit false find floor foreach foreachvar format frexp getbinmethod getenv getglobal gettagmethod gfind gmatch gsub insert ipairs ldexp log log10 match max min mod newtag next nextvar os pairs print rad random randomseed rawgetglobal rawgettable rawsetglobal rawsettable read readfrom remove rename seterrormethod setglobal setlocale settag settagmethod sin sqrt strbyte sub strchar strfind string strlen strlower strrep strsub strupper table tag tan tmpname tonumber tostring true type write writeto";
	System:setLuaKeyWord(szkeyword);
	System:setLuaConstantWord(szConst);
end

--根据类型,判断是否,需要显示添加函数的对话框
function canShowFunctionDlg(windowType)
	if windowType == "Falagard/ActionButton" then
		return 1;
	elseif windowType == "Falagard/Animate" then
		return 1;
	elseif windowType == "Falagard/CheckBox" then
		return 1;
	elseif windowType == "Falagard/SystemButton" then
		return 1;
	elseif windowType == "Falagard/Button" then
		return 1;
	elseif windowType == "Falagard/Combobox" then
		return 1;
	elseif windowType == "Falagard/RadioButton" then
		return 1;
	elseif windowType == "Falagard/TabControl" then
		return 1;
	end
	return 0;
end





---属性相关的函数,写在下面

function getBasePropList(setName)

end


--获取特定窗口的属性
function getSpecialWindowPropertyList(windowType, szSetName)

	if windowType == "Falagard/ActionButton" then
		getActionButtonProperty(szSetName);
		getPushButtonProperty(szSetName);
		getButtonBaseProperty(szSetName);
	elseif windowType == "Falagard/MeshWindow" then
		getMeshWindowProperty(szSetName);
	elseif windowType == "Falagard/SoftKeyWindow" then
		getSoftkeyBoardProperty(szSetName);
	elseif windowType == "Falagard/SceneMap" then
		getSceneMapProperty(szSetName);
	elseif windowType == "Falagard/Animate" then
		getPushButtonProperty(szSetName);
	elseif windowType == "Falagard/CheckBox" then
		getCheckBoxProperty(szSetName);
	elseif windowType == "Falagard/SystemButton" then
		
	elseif windowType == "Falagard/RewardItem" then
		
	elseif windowType == "Falagard/SuperProgress" then
		getProgressBarProperty(szSetName);
		getSupperProgressBarProperty(szSetName);
	elseif windowType == "Falagard/IMEEditBox" then
		getEditBoxProperty(szSetName);
		getIMEEditboxProperty(szSetName);
	elseif windowType == "Falagard/MultiIMEEditBox" then
		getMutiEditboxProperty(szSetName);
		getMultiLineEditboxProperty(szSetName);
	elseif windowType == "Falagard/Tree" then
		getTreeProperty(szSetName);
	elseif windowType == "Falagard/Button" then
		getPushButtonProperty(szSetName);
		getButtonBaseProperty(szSetName);
	elseif windowType == "Falagard/Combobox" then
		getComBoxProperty(szSetName);
	elseif windowType == "Falagard/Editbox" then
		getEditBoxProperty(szSetName);
	elseif windowType == "Falagard/MultiLineEditbox" then
		getMultiLineEditboxProperty(szSetName);
	elseif windowType == "Falagard/FrameWindow" then
		getFrameWindowProperty(szSetName);
	elseif windowType == "Falagard/Listbox" then
		getListBoxProperty(szSetName);		
	elseif windowType == "Falagard/Menubar" then
		getMenuProperty(szSetName);
	elseif windowType == "Falagard/MultiColumnList" then
		getMutiColumnListProperty(szSetName);
	elseif windowType == "Falagard/PopupMenu" then
		getMenuProperty(szSetName);
		getPopMenuProperty(szSetName);
	elseif windowType == "Falagard/ProgressBar" then
		getProgressBarProperty(szSetName);
	elseif windowType == "Falagard/RadioButton" then
		getRadioProperty(szSetName);
	elseif windowType == "Falagard/ScrollablePane" then
		getScrollBarPanel(szSetName);
	elseif windowType == "Falagard/Scrollbar" then
		getScrollBarProperty(szSetName);
	elseif windowType == "Falagard/Slider" then
		getSliderProperty(szSetName);
	elseif windowType == "Falagard/Spinner" then
		getSpinnerProperty(szSetName);
	elseif windowType == "Falagard/TabControl" then
		getTabControlProperty(szSetName);
	elseif windowType == "Falagard/Thumb" then
		
	elseif windowType == "Falagard/Tooltip" then
		
	else
		System:print("缺少扩展属性实现:"..windowType .."\n")
	end
	--System:insertEditItem("Empty","占位防止最后一个无法编辑,不用设置",szSetName);
end


--获取基本按钮窗口的属性
function getButtonBaseProperty(szSetName)

	System:insertColorDlgItem("NormalTextColour","普通文本颜色",szSetName);
	System:insertColorDlgItem("HoverTextColour","高亮文本颜色",szSetName);
	System:insertColorDlgItem("PushedTextColour","按下文本颜色",szSetName);
	System:insertColorDlgItem("DisabledTextColour","禁用文本颜色",szSetName);
	
end

--获取PushButton的属性
function getPushButtonProperty(szSetName)

	System:insertEditItem("NormalImage","普通状态图片",szSetName);
	System:insertEditItem("PushedImage","高亮状态图片",szSetName);
	System:insertEditItem("HoverImage","按下状态图片",szSetName);
	System:insertEditItem("DisabledImage","禁用状态图片",szSetName);

end

--获取ActionButton的属性
function getActionButtonProperty(szSetName)
	System:insertEditItem("DragAcceptName","拖拽的名称,这个必须填写",szSetName);
end

--获取Static控件的属性
function getStaticProperty(szSetName)

	System:insertEditItem("BackgroundImage","背景图片(在最中间)",szSetName);
	System:insertEditItem("TopLeftFrameImage","左上角图片",szSetName);
	System:insertEditItem("TopRightFrameImage","右上角图片",szSetName);
	System:insertEditItem("BottomLeftFrameImage","左下角图片",szSetName);
	
	System:insertEditItem("BottomRightFrameImage","右下角图片",szSetName);
	System:insertEditItem("LeftFrameImage","左边中间图片",szSetName);
	System:insertEditItem("RightFrameImage","右边中间图片",szSetName);
	System:insertEditItem("TopFrameImage","上边中间图片",szSetName);
	System:insertEditItem("BottomFrameImage","下边中间图片",szSetName);
	
	System:insertComboboxItem("FrameEnabled","框架是否激活",szSetName,"True", "False");
	System:insertComboboxItem("BackgroundEnabled","背景是否激活",szSetName,"True", "False");
	System:insertColorDlgItem("FrameColours","框架颜色",szSetName);
	System:insertColorDlgItem("BackgroundColours","背景颜色",szSetName);
end

--获取StaticImage控件属性
function getStaticImageProperty(szSetName)

	System:insertComboboxItem("VertFormatting","竖直格式",szSetName,"VertStretched","TopAligned", "BottomAligned","VertTiled", "VertCentred");
	System:insertComboboxItem("HorzFormatting","竖直格式",szSetName,"HorzStretched","LeftAligned", "RightAligned","HorzTiled", "HorzCentred");
	
	System:insertEditItem("RightAnchor","右边停靠的窗口名称",szSetName);
	System:insertEditItem("BottomAnchor","低边停靠的窗口名称",szSetName);
	
	System:insertEditItem("Image","图像集合",szSetName);
	System:insertColorDlgItem("ImageColours","图像颜色",szSetName);
end

--获取MeshWindow的控件实现
function getMeshWindowProperty(szSetName)

	System:insertEditItem("FakeObject","FakeObject对象名称",szSetName);
	System:insertEditItem("ShowControl","显示FakeObject对象的窗口名称",szSetName);
	
end

--获取SceneMap的控件属性
function getSceneMapProperty(szSetName)

	System:insertEditItem("PlayerImage","玩家图片",szSetName);
	System:insertEditItem("MySelfImage","自己显示的图片",szSetName);

end

--获取keyboard的控件实现
function getSoftkeyBoardProperty(szSetName)

	--System:insertEditItem("HoverImage","高亮的图片",szSetName);
	--System:insertEditItem("PushedImage","按下的图片",szSetName);
	System:insertEditItem("KeySize","按键尺寸",szSetName);
	
end

--获取CheckBox的控件属性
function getCheckBoxProperty(szSetName)
	System:insertComboboxItem("Selected","是否选中",szSetName,"True", "False");
end

--获取ProgressBar的控件属性
function getProgressBarProperty(szSetName)

	System:insertEditItem("CurrentProgress","当前进度",szSetName);
	System:insertEditItem("StepSize","步长",szSetName);

end

--获取SupperProgressBar的属性
function getSupperProgressBarProperty(szSetName)

	System:insertEditItem("ProgressImage","当前进度",szSetName);
	System:insertEditItem("EffectColorLevel","步长",szSetName);
	System:insertEditItem("MoveSpeed","移动速度",szSetName);
	System:insertEditItem("EffectColor","特效颜色",szSetName);
		
end

--获取Editbox的属性
function getEditBoxProperty(szSetName)
	
	System:insertComboboxItem("ReadOnly","是否只读",szSetName,"True", "False");	
	--System:insertEditItem("MaskText","掩码",szSetName);
	System:insertEditItem("ValidationString","正则表达式",szSetName);
	System:insertEditItem("MaxTextLength","最大字符长度(汉字算两个)",szSetName);
	System:insertColorDlgItem("NormalTextColour","正常文本颜色",szSetName);
	System:insertColorDlgItem("SelectedTextColour","选择文本颜色",szSetName);
	System:insertColorDlgItem("ActiveSelectionColour","激活文本颜色",szSetName);
	System:insertColorDlgItem("InactiveSelectionColour","失去激活状态文本颜色",szSetName);
	
end

--获取IMEEidtbox的属性
function getIMEEditboxProperty(szSetName)

	System:insertComboboxItem("DefaultEditBox","是否默认的编辑框",szSetName,"True", "False");
	
end

--获取MultiLineEditbox的属性
function getMultiLineEditboxProperty(szSetName)

	System:insertComboboxItem("ReadOnly","是否只读",szSetName,"True", "False");	
	System:insertComboboxItem("WordWrap","是否容许文字自动换行",szSetName,"True", "False");
	System:insertEditItem("MaxTextLength","最大字符长度(汉字算两个)",szSetName);
	System:insertColorDlgItem("NormalTextColour","正常文本颜色",szSetName);
	System:insertColorDlgItem("SelectedTextColour","选择文本颜色",szSetName);
	System:insertColorDlgItem("ActiveSelectionColour","激活文本颜色",szSetName);
	System:insertColorDlgItem("InactiveSelectionColour","失去激活状态文本颜色",szSetName);
	System:insertComboboxItem("OnReturnEnable","是否容许回车换行",szSetName,"True", "False");

end

--获取多行列表框的属性
function getMutiEditboxProperty(szSetName)

	System:insertEditItem("SelectionBrushImage","选择使用的图片画刷",szSetName);
	
end

--获取Tree的属性
function getTreeProperty(szSetName)

	System:insertComboboxItem("Sort","是否排序",szSetName,"True", "False");	
	System:insertComboboxItem("MultiSelect","多重排序",szSetName,"True", "False");	               
	System:insertComboboxItem("ForceVertScrollbar","强制显示竖直滚动条",szSetName,"True", "False");	               
	System:insertComboboxItem("ForceHorzScrollbar","强制显示水平滚动条",szSetName,"True", "False");	               
	System:insertComboboxItem("ItemTooltips","是否支持子项Tooltip",szSetName,"True", "False");	               

end

--获取ComBox的属性
function getComBoxProperty(szSetName)

	getEditBoxProperty(szSetName)
	
	System:insertComboboxItem("ForceVertScrollbar","强制显示竖直滚动条",szSetName,"True", "False");	               
	System:insertComboboxItem("ForceHorzScrollbar","强制显示水平滚动条",szSetName,"True", "False");
	 
	System:insertComboboxItem("SortList","是否排序",szSetName,"True", "False");	
	--System:insertComboboxItem("SingleClickMode","是否排序",szSetName,"True", "False");	

end

--获取FrameWindow的属性
function getFrameWindowProperty(szSetName)

	System:insertEditItem("NSSizingCursorImage","上下调整时候的鼠标图片",szSetName);
	System:insertEditItem("EWSizingCursorImage","东西调整时候的鼠标图片",szSetName);
	System:insertEditItem("NWSESizingCursorImage","西北东南方向调整的鼠标图片",szSetName);
	System:insertEditItem("NESWSizingCursorImage","东北西南方向调整的鼠标图片",szSetName);
	
	System:insertComboboxItem("SizingEnabled","是否可以缩放窗口",szSetName,"True", "False");
	System:insertComboboxItem("FrameEnabled","是否支持框架",szSetName,"True", "False");	            
	System:insertComboboxItem("TitlebarEnabled","是否支持标题栏",szSetName,"True", "False");	            
	System:insertComboboxItem("CloseButtonEnabled","是否包含关闭按钮",szSetName,"True", "False");	            
	System:insertComboboxItem("DragMovingEnabled","是否支持拖拽移动",szSetName,"True", "False");	      
end

--获取ListBox的属性吗
function getListBoxProperty(szSetName)
	
	System:insertComboboxItem("Sort","是否支持排序",szSetName,"True", "False");
	System:insertComboboxItem("MultiSelect","是否支持排序",szSetName,"True", "False");
	System:insertComboboxItem("ForceVertScrollbar","强制显示竖直滚动条",szSetName,"True", "False");
	System:insertComboboxItem("ForceHorzScrollbar","强制显示水平滚动条",szSetName,"True", "False");
	System:insertComboboxItem("ItemTooltips","子项Tooltip",szSetName,"True", "False");
	
end

--获取基本菜单的属性
function getMenuProperty(szSetName)

	System:insertComboboxItem("AutoResizeEnabled","是否支持自动缩放窗口",szSetName,"True", "False");
	
	System:insertEditItem("ItemSpacing","子项间距",szSetName);
	System:insertEditItem("HorzPadding","水平空白",szSetName);
	System:insertEditItem("VertPadding","竖直空白",szSetName);
	System:insertEditItem("BorderWidth","边界宽度",szSetName);
	System:insertColorDlgItem("BackgroundColours","背景颜色",szSetName);
	System:insertComboboxItem("AllowMultiplePopups","是否支持子菜单",szSetName,"True", "False");
	
end


--获取多行列表框
function getMutiColumnListProperty(szSetName)
	
	System:insertComboboxItem("ColumnsMovable","是否支持列移动",szSetName,"True", "False");
	System:insertComboboxItem("ColumnsSizable","是否支持调整列宽度",szSetName,"True", "False");
	System:insertComboboxItem("ForceHorzScrollbar","是否强制竖直滚动条出现",szSetName,"True", "False");
	System:insertComboboxItem("ForceVertScrollbar","是否强制水平滚动条出现",szSetName,"True", "False");
	System:insertComboboxItem("SortSettingEnabled","是否支持排序",szSetName,"True", "False");
	System:insertComboboxItem("SelectionMode","选择模式",szSetName,"RowSingle", "RowMultiple","ColumnSingle","ColumnMultiple","CellSingle","CellMultiple","NominatedColumnSingle","NominatedColumnMultiple","NominatedRowSingle","NominatedRowMultiple");
	System:insertComboboxItem("AllowMultiplePopups","是否支持子菜单",szSetName,"True", "False");
	
	System:insertEditItem("NominatedSelectionColumnID","和排序相关列",szSetName);
	System:insertEditItem("NominatedSelectionRow","和排序相关的行",szSetName);
	
	System:insertEditItem("SortColumnID","根据那一列排序",szSetName);
	System:insertComboboxItem("SortDirection","排序类型",szSetName,"None", "Ascending","Descending");

end

--获取PopMenu的属性
function getPopMenuProperty(szSetName)

	System:insertEditItem("FadeInTime","渐现时间",szSetName);
	System:insertEditItem("FadeOutTime","渐隐时间",szSetName);
		
end

--获取Radio的属性
function getRadioProperty(szSetName)

	System:insertEditItem("GroupID","组ID(同一组只有一个可以被选中)",szSetName);
	System:insertComboboxItem("Selected","是否选中",szSetName,"True", "False");
end

--获取ScrollBarPanel的属性
function getScrollBarPanel(szSetName)

	System:insertEditItem("ContentArea","内容区域",szSetName);
	System:insertEditItem("HorzStepSize","水平步长",szSetName);
	System:insertEditItem("HorzOverlapSize","水平...",szSetName);
	System:insertEditItem("HorzScrollPosition","水平滚动条位置",szSetName);
	System:insertEditItem("VertStepSize","竖直步长",szSetName);
	System:insertEditItem("VertOverlapSize","竖直..",szSetName);
	System:insertEditItem("VertScrollPosition","竖直滚动条位置",szSetName);
	
	System:insertComboboxItem("ForceHorzScrollbar","是否强制竖直滚动条出现",szSetName,"True", "False");
	System:insertComboboxItem("ForceVertScrollbar","是否强制水平滚动条出现",szSetName,"True", "False");
	System:insertComboboxItem("ContentPaneAutoSized","面板是否自动缩放",szSetName,"True", "False");
	
end

--获取ScrollBar的属性
function getScrollBarProperty(szSetName)

	System:insertEditItem("DocumentSize","文档长度",szSetName);
	System:insertEditItem("PageSize","每页的高度",szSetName);
	System:insertEditItem("StepSize","步长大小",szSetName);
	System:insertEditItem("OverlapSize","不知道怎么描述,自己看着办吧,或者来修改我",szSetName);
	System:insertEditItem("ScrollPosition","滚动条位置",szSetName);
		
end

--获取Slider的属性
function getSliderProperty(szSetName)

	System:insertEditItem("CurrentValue","当前值",szSetName);
	System:insertEditItem("MaximumValue","最大值",szSetName);
	System:insertEditItem("ClickStepSize","单击步长大小",szSetName);
	
end

--获取Spinner的属性
function getSpinnerProperty(szSetName)

	System:insertEditItem("CurrentValue","当前值",szSetName);
	System:insertEditItem("MaximumValue","最大值",szSetName);
	System:insertEditItem("MinimumValue","最大值",szSetName);
	System:insertEditItem("StepSize","步长大小",szSetName);
	
	System:insertComboboxItem("TextInputMode","文本输入模式",szSetName,"Integer", "Octal","Hexadecimal","FloatingPoint");

end

--获取TabControl的属性
function getTabControlProperty(szSetName)
	
	System:insertEditItem("TabHeight","Tab高度",szSetName);
	--System:insertEditItem("AbsoluteTabHeight","绝对高度",szSetName);
	--System:insertEditItem("RelativeTabHeight","相对高度",szSetName);
	System:insertEditItem("TabTextPadding","文本Padding",szSetName);
	System:insertEditItem("AbsoluteTabTextPadding","绝对Padding",szSetName);
	System:insertEditItem("RelativeTabTextPadding","相对Padding",szSetName);
		
end


--当选择一个窗口的时候,会调用这个函数,把这个窗口的对应值刷新到界面中,这个函数需要返回属性列表
function onUpdateSpecialProperty(windowType)

	if windowType == "Falagard/ActionButton" then
		return "NormalTextColour HoverTextColour PushedTextColour DisabledTextColour NormalImage PushedImage DisabledImage DragAcceptName";
	elseif windowType == "Falagard/MeshWindow" then
		return "FakeObject ShowControl";
	elseif windowType == "Falagard/SoftKeyWindow" then
		return "KeySize";
	elseif windowType == "Falagard/SceneMap" then
		return "PlayerImage MySelfImage";
	elseif windowType == "Falagard/Animate" then
		return "NormalImage PushedImage DisabledImage";
	elseif windowType == "Falagard/CheckBox" then
		return "Selected";
	elseif windowType == "Falagard/SystemButton" then
		
	elseif windowType == "Falagard/RewardItem" then
		
	elseif windowType == "Falagard/SuperProgress" then
		return "CurrentProgress StepSize ProgressImage EffectColorLevel MoveSpeed EffectColor";
	elseif windowType == "Falagard/IMEEditBox" then
		return "ReadOnly MaskText ValidationString MaxTextLength NormalTextColour SelectedTextColour ActiveSelectionColour InactiveSelectionColour DefaultEditBox";
	elseif windowType == "Falagard/MultiIMEEditBox" then
		return "ReadOnly WordWrap MaxTextLength NormalTextColour SelectedTextColour ActiveSelectionColour InactiveSelectionColour OnReturnEnable SelectionBrushImage";
	elseif windowType == "Falagard/Tree" then
		return "Sort MultiSelect ForceVertScrollbar ForceHorzScrollbar ItemTooltips";
	elseif windowType == "Falagard/Button" then
		return "NormalTextColour HoverTextColour PushedTextColour DisabledTextColour NormalImage PushedImage DisabledImage";
	elseif windowType == "Falagard/Combobox" then
		return "ForceVertScrollbar ForceHorzScrollbar SortList ReadOnly MaskText ValidationString MaxTextLength NormalTextColour SelectedTextColour ActiveSelectionColour InactiveSelectionColour";
	elseif windowType == "Falagard/Editbox" then
		return "ReadOnly MaskText ValidationString MaxTextLength NormalTextColour SelectedTextColour ActiveSelectionColour InactiveSelectionColour";
	elseif windowType == "Falagard/MultiLineEditbox" then
		return "editbox ReadOnly WordWrap MaxTextLength NormalTextColour SelectedTextColour ActiveSelectionColour InactiveSelectionColour OnReturnEnable";
	elseif windowType == "Falagard/FrameWindow" then
		return "SizingEnabled FrameEnabled TitlebarEnabled CloseButtonEnabled DragMovingEnabled NSSizingCursorImage EWSizingCursorImage NWSESizingCursorImage NESWSizingCursorImage";
	elseif windowType == "Falagard/Listbox" then
		return "MultiSelect ForceVertScrollbar ForceHorzScrollbar ItemTooltips";	
	elseif windowType == "Falagard/Menubar" then
		return "AutoResizeEnabled ItemSpacing HorzPadding VertPadding BorderWidth BackgroundColours AllowMultiplePopups";
	elseif windowType == "Falagard/MultiColumnList" then
		return "ColumnsMovable ColumnsSizable ForceHorzScrollbar ForceVertScrollbar SortSettingEnabled SelectionMode AllowMultiplePopups NominatedSelectionColumnID NominatedSelectionRow SortColumnID SortDirection";
	elseif windowType == "Falagard/PopupMenu" then
		return "FadeInTime FadeOutTime AutoResizeEnabled ItemSpacing HorzPadding VertPadding BorderWidth BackgroundColours AllowMultiplePopups";
	elseif windowType == "Falagard/ProgressBar" then
		return "CurrentProgress StepSize";
	elseif windowType == "Falagard/RadioButton" then
		return "Selected GroupID";
	elseif windowType == "Falagard/ScrollablePane" then
		return "ContentArea HorzStepSize HorzOverlapSize HorzScrollPosition VertStepSize VertOverlapSize VertScrollPosition ForceHorzScrollbar ForceVertScrollbar ContentPaneAutoSized";
	elseif windowType == "Falagard/Scrollbar" then
		return "DocumentSize PageSize StepSize OverlapSize ScrollPosition";
	elseif windowType == "Falagard/Slider" then
		return "CurrentValue MaximumValue ClickStepSize";
	elseif windowType == "Falagard/Spinner" then
		return "CurrentValue MaximumValue MinimumValue StepSize";
	elseif windowType == "Falagard/TabControl" then
		return "TabHeight TabTextPadding RelativeTabTextPadding AbsoluteTabTextPadding";
	elseif windowType == "Falagard/Thumb" then
		
	elseif windowType == "Falagard/Tooltip" then
		
	else
		return "";
	end
	return "";
end



--获取Toolbar工具
function getToolBarNameByType(winType)
	if winType == "WoWLook/MultiColumnList" then
		return "多行列表框";
	elseif winType == "WoWLook/SoftKeyWindow" then
		return "软键盘窗口";
	elseif winType == "WoWLook/Slider" then
		return "Slider窗口";
	elseif winType == "WoWLook/PaoPao" then
		return "冒泡窗口";
	elseif winType == "WoWLook/SelfFitWindow" then
		return "自适应文本窗口";
	elseif winType == "WoWLook/AnimateWindow" then
		return "动画窗口1";
	elseif winType == "WoWLook/Animate" then
		return "动画窗口2";
	elseif winType == "WoWLook/StopWatch" then
		return "倒计时窗口";
	elseif winType == "WoWLook/CreatureBoard" then
		return "头顶信息板";
	elseif winType == "WoWLook/Minimap" then
		return "MiniMap窗口";
	elseif winType == "WoWLook/InfoList" then
		return "InfoList窗口";
	elseif winType == "WoWLook/WorldMap" then
		return "世界地图窗口";
	elseif winType == "WoWLook/SceneMap" then
		return "场景地图窗口";
	elseif winType == "WoWLook/SuperProgress" then
		return "超级进度条窗口";
	elseif winType == "WoWLook/MeshWindow" then
		return "界面显示Avtar窗口";
	elseif winType == "WoWLook/SimpleText" then
		return "普通文本窗口";
	elseif winType == "WoWLook/Money" then
		return "金钱控件窗口";
	elseif winType == "WoWLook/Button_Combo" then
		return "Combo 窗口";
	elseif winType == "WoWLook/ComboEditbox" then
		return "Combo 窗口(可编辑)";
	elseif winType == "WoWLook/ComboList" then
		return "Combo 窗口(带列表)";
	elseif winType == "WoWLook/ComboDropList" then
		return "Combo 窗口(带列表)";
	elseif winType == "WoWLook/Listbox_Debug" then
		return "列表提示条窗口";
	elseif winType == "WoWLook/Listbox" then
		return "普通列表框窗口";
	elseif winType == "WoWLook/SuperTooltip" then
		return "超级Tooltip";
	elseif winType == "WoWLook/Tooltip" then
		return "普通Tooltip";
	elseif winType == "WoWLook/PopupMenu" then
		return "弹出菜单窗口";
	elseif winType == "WoWLook/MenuItem" then
		return "菜单项窗口";
	elseif winType == "WoWLook/SystemButton" then
		return "系统按钮窗口";
	elseif winType == "WoWLook/Titlebar_2" then
		return "标题窗口2";
	elseif winType == "WoWLook/Titlebar" then
		return "标题窗口1";
	elseif winType == "WoWLook/FrameWindow_HelpText" then
		return "框架窗口带帮助";
	elseif winType == "WoWLook/Button_MiniClose" then
		return "MiniClose按钮";
	elseif winType == "WoWLook/HorizontalScrollbar" then
		return "水平滚动条";
	elseif winType == "WoWLook/VerticalScrollbar" then
		return "垂直滚动条";
	elseif winType == "WoWLook/StaticImage" then
		return "静态图像窗口";
	elseif winType == "WoWLook/StaticText3" then
		return "静态文本3";
	elseif winType == "WoWLook/StaticText2" then
		return "静态文本2";
	elseif winType == "WoWLook/StaticText" then
		return "静态文本1";
	elseif winType == "WoWLook/MultiIMEEditBox" then
		return "多行输入框";
	elseif winType == "WoWLook/ChatEditbox_IME" then
		return "聊天输入窗口";
	elseif winType == "WoWLook/IMEEditBox" then
		return "支持中文的单行输入框";
	elseif winType == "WoWLook/Editbox_1" then
		return "普通输入框1";
	elseif winType == "WoWLook/Editbox_2" then
		return "普通输入框2";
	elseif winType == "WoWLook/Editbox_3" then
		return "普通输入框3";
	elseif winType == "WoWLook/Editbox_4" then
		return "普通输入框4";
	elseif winType == "WoWLook/Editbox_5" then
		return "普通输入框5";
	elseif winType == "WoWLook/Editbox_6" then
		return "普通输入框6";
	elseif winType == "WoWLook/ActionButton" then
		return "ActionButton窗口";
	elseif winType == "WoWLook/Button_Text" then
		return "带文本的Button窗口";
	elseif winType == "WoWLook/Button_1" then
		return "按钮1";
	elseif winType == "WoWLook/Button_2" then
		return "按钮2";
	elseif winType == "WoWLook/Button_3" then
		return "按钮3";
	elseif winType == "WoWLook/Button_4" then
		return "按钮4";
	elseif winType == "WoWLook/Button_5" then
		return "按钮5";
	elseif winType == "WoWLook/Button_6" then
		return "按钮6";
	elseif winType == "WoWLook/Button_7" then
		return "按钮7";
	elseif winType == "WoWLook/Button_8" then
		return "按钮8";
	elseif winType == "WoWLook/ButtonDrop" then
		return "Drop按钮";
	elseif winType == "WoWLook/ButtonSale" then
		return "Sale按钮";
	elseif winType == "WoWLook/FrameWindow" then
		return "框架窗口";
	end
	return "";
end

