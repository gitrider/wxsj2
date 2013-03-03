


--��ȡ��ͨ�Ĵ����¼�,����дWindow���ڵ�,Window���ڵ���C++�����Ѿ�֧��
function getCommonWindowEventList()


end

--������ĳ���ض����ʹ��ڵ�List,����windowType��ͬ,����ִ�в�ͬ�����

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
		System:print("ȱ����չ�¼�:"..windowType .."\n")
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


--��ȡlua�Ĺؼ��ʺͳ���
function getLuaKeywordAndConst()
	local szkeyword = "and break do else  elseif end function if in local nil not or repeat return then until while this";
	local szConst = "abs acos appendto ascii asin assert atan atan2 call ceil clock collectgarbage copytagmethods cos date deg dofile dostring error execute exit false find floor foreach foreachvar format frexp getbinmethod getenv getglobal gettagmethod gfind gmatch gsub insert ipairs ldexp log log10 match max min mod newtag next nextvar os pairs print rad random randomseed rawgetglobal rawgettable rawsetglobal rawsettable read readfrom remove rename seterrormethod setglobal setlocale settag settagmethod sin sqrt strbyte sub strchar strfind string strlen strlower strrep strsub strupper table tag tan tmpname tonumber tostring true type write writeto";
	System:setLuaKeyWord(szkeyword);
	System:setLuaConstantWord(szConst);
end

--��������,�ж��Ƿ�,��Ҫ��ʾ��Ӻ����ĶԻ���
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





---������صĺ���,д������

function getBasePropList(setName)

end


--��ȡ�ض����ڵ�����
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
		System:print("ȱ����չ����ʵ��:"..windowType .."\n")
	end
	--System:insertEditItem("Empty","ռλ��ֹ���һ���޷��༭,��������",szSetName);
end


--��ȡ������ť���ڵ�����
function getButtonBaseProperty(szSetName)

	System:insertColorDlgItem("NormalTextColour","��ͨ�ı���ɫ",szSetName);
	System:insertColorDlgItem("HoverTextColour","�����ı���ɫ",szSetName);
	System:insertColorDlgItem("PushedTextColour","�����ı���ɫ",szSetName);
	System:insertColorDlgItem("DisabledTextColour","�����ı���ɫ",szSetName);
	
end

--��ȡPushButton������
function getPushButtonProperty(szSetName)

	System:insertEditItem("NormalImage","��ͨ״̬ͼƬ",szSetName);
	System:insertEditItem("PushedImage","����״̬ͼƬ",szSetName);
	System:insertEditItem("HoverImage","����״̬ͼƬ",szSetName);
	System:insertEditItem("DisabledImage","����״̬ͼƬ",szSetName);

end

--��ȡActionButton������
function getActionButtonProperty(szSetName)
	System:insertEditItem("DragAcceptName","��ק������,���������д",szSetName);
end

--��ȡStatic�ؼ�������
function getStaticProperty(szSetName)

	System:insertEditItem("BackgroundImage","����ͼƬ(�����м�)",szSetName);
	System:insertEditItem("TopLeftFrameImage","���Ͻ�ͼƬ",szSetName);
	System:insertEditItem("TopRightFrameImage","���Ͻ�ͼƬ",szSetName);
	System:insertEditItem("BottomLeftFrameImage","���½�ͼƬ",szSetName);
	
	System:insertEditItem("BottomRightFrameImage","���½�ͼƬ",szSetName);
	System:insertEditItem("LeftFrameImage","����м�ͼƬ",szSetName);
	System:insertEditItem("RightFrameImage","�ұ��м�ͼƬ",szSetName);
	System:insertEditItem("TopFrameImage","�ϱ��м�ͼƬ",szSetName);
	System:insertEditItem("BottomFrameImage","�±��м�ͼƬ",szSetName);
	
	System:insertComboboxItem("FrameEnabled","����Ƿ񼤻�",szSetName,"True", "False");
	System:insertComboboxItem("BackgroundEnabled","�����Ƿ񼤻�",szSetName,"True", "False");
	System:insertColorDlgItem("FrameColours","�����ɫ",szSetName);
	System:insertColorDlgItem("BackgroundColours","������ɫ",szSetName);
end

--��ȡStaticImage�ؼ�����
function getStaticImageProperty(szSetName)

	System:insertComboboxItem("VertFormatting","��ֱ��ʽ",szSetName,"VertStretched","TopAligned", "BottomAligned","VertTiled", "VertCentred");
	System:insertComboboxItem("HorzFormatting","��ֱ��ʽ",szSetName,"HorzStretched","LeftAligned", "RightAligned","HorzTiled", "HorzCentred");
	
	System:insertEditItem("RightAnchor","�ұ�ͣ���Ĵ�������",szSetName);
	System:insertEditItem("BottomAnchor","�ͱ�ͣ���Ĵ�������",szSetName);
	
	System:insertEditItem("Image","ͼ�񼯺�",szSetName);
	System:insertColorDlgItem("ImageColours","ͼ����ɫ",szSetName);
end

--��ȡMeshWindow�Ŀؼ�ʵ��
function getMeshWindowProperty(szSetName)

	System:insertEditItem("FakeObject","FakeObject��������",szSetName);
	System:insertEditItem("ShowControl","��ʾFakeObject����Ĵ�������",szSetName);
	
end

--��ȡSceneMap�Ŀؼ�����
function getSceneMapProperty(szSetName)

	System:insertEditItem("PlayerImage","���ͼƬ",szSetName);
	System:insertEditItem("MySelfImage","�Լ���ʾ��ͼƬ",szSetName);

end

--��ȡkeyboard�Ŀؼ�ʵ��
function getSoftkeyBoardProperty(szSetName)

	--System:insertEditItem("HoverImage","������ͼƬ",szSetName);
	--System:insertEditItem("PushedImage","���µ�ͼƬ",szSetName);
	System:insertEditItem("KeySize","�����ߴ�",szSetName);
	
end

--��ȡCheckBox�Ŀؼ�����
function getCheckBoxProperty(szSetName)
	System:insertComboboxItem("Selected","�Ƿ�ѡ��",szSetName,"True", "False");
end

--��ȡProgressBar�Ŀؼ�����
function getProgressBarProperty(szSetName)

	System:insertEditItem("CurrentProgress","��ǰ����",szSetName);
	System:insertEditItem("StepSize","����",szSetName);

end

--��ȡSupperProgressBar������
function getSupperProgressBarProperty(szSetName)

	System:insertEditItem("ProgressImage","��ǰ����",szSetName);
	System:insertEditItem("EffectColorLevel","����",szSetName);
	System:insertEditItem("MoveSpeed","�ƶ��ٶ�",szSetName);
	System:insertEditItem("EffectColor","��Ч��ɫ",szSetName);
		
end

--��ȡEditbox������
function getEditBoxProperty(szSetName)
	
	System:insertComboboxItem("ReadOnly","�Ƿ�ֻ��",szSetName,"True", "False");	
	--System:insertEditItem("MaskText","����",szSetName);
	System:insertEditItem("ValidationString","������ʽ",szSetName);
	System:insertEditItem("MaxTextLength","����ַ�����(����������)",szSetName);
	System:insertColorDlgItem("NormalTextColour","�����ı���ɫ",szSetName);
	System:insertColorDlgItem("SelectedTextColour","ѡ���ı���ɫ",szSetName);
	System:insertColorDlgItem("ActiveSelectionColour","�����ı���ɫ",szSetName);
	System:insertColorDlgItem("InactiveSelectionColour","ʧȥ����״̬�ı���ɫ",szSetName);
	
end

--��ȡIMEEidtbox������
function getIMEEditboxProperty(szSetName)

	System:insertComboboxItem("DefaultEditBox","�Ƿ�Ĭ�ϵı༭��",szSetName,"True", "False");
	
end

--��ȡMultiLineEditbox������
function getMultiLineEditboxProperty(szSetName)

	System:insertComboboxItem("ReadOnly","�Ƿ�ֻ��",szSetName,"True", "False");	
	System:insertComboboxItem("WordWrap","�Ƿ����������Զ�����",szSetName,"True", "False");
	System:insertEditItem("MaxTextLength","����ַ�����(����������)",szSetName);
	System:insertColorDlgItem("NormalTextColour","�����ı���ɫ",szSetName);
	System:insertColorDlgItem("SelectedTextColour","ѡ���ı���ɫ",szSetName);
	System:insertColorDlgItem("ActiveSelectionColour","�����ı���ɫ",szSetName);
	System:insertColorDlgItem("InactiveSelectionColour","ʧȥ����״̬�ı���ɫ",szSetName);
	System:insertComboboxItem("OnReturnEnable","�Ƿ�����س�����",szSetName,"True", "False");

end

--��ȡ�����б�������
function getMutiEditboxProperty(szSetName)

	System:insertEditItem("SelectionBrushImage","ѡ��ʹ�õ�ͼƬ��ˢ",szSetName);
	
end

--��ȡTree������
function getTreeProperty(szSetName)

	System:insertComboboxItem("Sort","�Ƿ�����",szSetName,"True", "False");	
	System:insertComboboxItem("MultiSelect","��������",szSetName,"True", "False");	               
	System:insertComboboxItem("ForceVertScrollbar","ǿ����ʾ��ֱ������",szSetName,"True", "False");	               
	System:insertComboboxItem("ForceHorzScrollbar","ǿ����ʾˮƽ������",szSetName,"True", "False");	               
	System:insertComboboxItem("ItemTooltips","�Ƿ�֧������Tooltip",szSetName,"True", "False");	               

end

--��ȡComBox������
function getComBoxProperty(szSetName)

	getEditBoxProperty(szSetName)
	
	System:insertComboboxItem("ForceVertScrollbar","ǿ����ʾ��ֱ������",szSetName,"True", "False");	               
	System:insertComboboxItem("ForceHorzScrollbar","ǿ����ʾˮƽ������",szSetName,"True", "False");
	 
	System:insertComboboxItem("SortList","�Ƿ�����",szSetName,"True", "False");	
	--System:insertComboboxItem("SingleClickMode","�Ƿ�����",szSetName,"True", "False");	

end

--��ȡFrameWindow������
function getFrameWindowProperty(szSetName)

	System:insertEditItem("NSSizingCursorImage","���µ���ʱ������ͼƬ",szSetName);
	System:insertEditItem("EWSizingCursorImage","��������ʱ������ͼƬ",szSetName);
	System:insertEditItem("NWSESizingCursorImage","�������Ϸ�����������ͼƬ",szSetName);
	System:insertEditItem("NESWSizingCursorImage","�������Ϸ�����������ͼƬ",szSetName);
	
	System:insertComboboxItem("SizingEnabled","�Ƿ�������Ŵ���",szSetName,"True", "False");
	System:insertComboboxItem("FrameEnabled","�Ƿ�֧�ֿ��",szSetName,"True", "False");	            
	System:insertComboboxItem("TitlebarEnabled","�Ƿ�֧�ֱ�����",szSetName,"True", "False");	            
	System:insertComboboxItem("CloseButtonEnabled","�Ƿ�����رհ�ť",szSetName,"True", "False");	            
	System:insertComboboxItem("DragMovingEnabled","�Ƿ�֧����ק�ƶ�",szSetName,"True", "False");	      
end

--��ȡListBox��������
function getListBoxProperty(szSetName)
	
	System:insertComboboxItem("Sort","�Ƿ�֧������",szSetName,"True", "False");
	System:insertComboboxItem("MultiSelect","�Ƿ�֧������",szSetName,"True", "False");
	System:insertComboboxItem("ForceVertScrollbar","ǿ����ʾ��ֱ������",szSetName,"True", "False");
	System:insertComboboxItem("ForceHorzScrollbar","ǿ����ʾˮƽ������",szSetName,"True", "False");
	System:insertComboboxItem("ItemTooltips","����Tooltip",szSetName,"True", "False");
	
end

--��ȡ�����˵�������
function getMenuProperty(szSetName)

	System:insertComboboxItem("AutoResizeEnabled","�Ƿ�֧���Զ����Ŵ���",szSetName,"True", "False");
	
	System:insertEditItem("ItemSpacing","������",szSetName);
	System:insertEditItem("HorzPadding","ˮƽ�հ�",szSetName);
	System:insertEditItem("VertPadding","��ֱ�հ�",szSetName);
	System:insertEditItem("BorderWidth","�߽���",szSetName);
	System:insertColorDlgItem("BackgroundColours","������ɫ",szSetName);
	System:insertComboboxItem("AllowMultiplePopups","�Ƿ�֧���Ӳ˵�",szSetName,"True", "False");
	
end


--��ȡ�����б��
function getMutiColumnListProperty(szSetName)
	
	System:insertComboboxItem("ColumnsMovable","�Ƿ�֧�����ƶ�",szSetName,"True", "False");
	System:insertComboboxItem("ColumnsSizable","�Ƿ�֧�ֵ����п��",szSetName,"True", "False");
	System:insertComboboxItem("ForceHorzScrollbar","�Ƿ�ǿ����ֱ����������",szSetName,"True", "False");
	System:insertComboboxItem("ForceVertScrollbar","�Ƿ�ǿ��ˮƽ����������",szSetName,"True", "False");
	System:insertComboboxItem("SortSettingEnabled","�Ƿ�֧������",szSetName,"True", "False");
	System:insertComboboxItem("SelectionMode","ѡ��ģʽ",szSetName,"RowSingle", "RowMultiple","ColumnSingle","ColumnMultiple","CellSingle","CellMultiple","NominatedColumnSingle","NominatedColumnMultiple","NominatedRowSingle","NominatedRowMultiple");
	System:insertComboboxItem("AllowMultiplePopups","�Ƿ�֧���Ӳ˵�",szSetName,"True", "False");
	
	System:insertEditItem("NominatedSelectionColumnID","�����������",szSetName);
	System:insertEditItem("NominatedSelectionRow","��������ص���",szSetName);
	
	System:insertEditItem("SortColumnID","������һ������",szSetName);
	System:insertComboboxItem("SortDirection","��������",szSetName,"None", "Ascending","Descending");

end

--��ȡPopMenu������
function getPopMenuProperty(szSetName)

	System:insertEditItem("FadeInTime","����ʱ��",szSetName);
	System:insertEditItem("FadeOutTime","����ʱ��",szSetName);
		
end

--��ȡRadio������
function getRadioProperty(szSetName)

	System:insertEditItem("GroupID","��ID(ͬһ��ֻ��һ�����Ա�ѡ��)",szSetName);
	System:insertComboboxItem("Selected","�Ƿ�ѡ��",szSetName,"True", "False");
end

--��ȡScrollBarPanel������
function getScrollBarPanel(szSetName)

	System:insertEditItem("ContentArea","��������",szSetName);
	System:insertEditItem("HorzStepSize","ˮƽ����",szSetName);
	System:insertEditItem("HorzOverlapSize","ˮƽ...",szSetName);
	System:insertEditItem("HorzScrollPosition","ˮƽ������λ��",szSetName);
	System:insertEditItem("VertStepSize","��ֱ����",szSetName);
	System:insertEditItem("VertOverlapSize","��ֱ..",szSetName);
	System:insertEditItem("VertScrollPosition","��ֱ������λ��",szSetName);
	
	System:insertComboboxItem("ForceHorzScrollbar","�Ƿ�ǿ����ֱ����������",szSetName,"True", "False");
	System:insertComboboxItem("ForceVertScrollbar","�Ƿ�ǿ��ˮƽ����������",szSetName,"True", "False");
	System:insertComboboxItem("ContentPaneAutoSized","����Ƿ��Զ�����",szSetName,"True", "False");
	
end

--��ȡScrollBar������
function getScrollBarProperty(szSetName)

	System:insertEditItem("DocumentSize","�ĵ�����",szSetName);
	System:insertEditItem("PageSize","ÿҳ�ĸ߶�",szSetName);
	System:insertEditItem("StepSize","������С",szSetName);
	System:insertEditItem("OverlapSize","��֪����ô����,�Լ����Ű��,�������޸���",szSetName);
	System:insertEditItem("ScrollPosition","������λ��",szSetName);
		
end

--��ȡSlider������
function getSliderProperty(szSetName)

	System:insertEditItem("CurrentValue","��ǰֵ",szSetName);
	System:insertEditItem("MaximumValue","���ֵ",szSetName);
	System:insertEditItem("ClickStepSize","����������С",szSetName);
	
end

--��ȡSpinner������
function getSpinnerProperty(szSetName)

	System:insertEditItem("CurrentValue","��ǰֵ",szSetName);
	System:insertEditItem("MaximumValue","���ֵ",szSetName);
	System:insertEditItem("MinimumValue","���ֵ",szSetName);
	System:insertEditItem("StepSize","������С",szSetName);
	
	System:insertComboboxItem("TextInputMode","�ı�����ģʽ",szSetName,"Integer", "Octal","Hexadecimal","FloatingPoint");

end

--��ȡTabControl������
function getTabControlProperty(szSetName)
	
	System:insertEditItem("TabHeight","Tab�߶�",szSetName);
	--System:insertEditItem("AbsoluteTabHeight","���Ը߶�",szSetName);
	--System:insertEditItem("RelativeTabHeight","��Ը߶�",szSetName);
	System:insertEditItem("TabTextPadding","�ı�Padding",szSetName);
	System:insertEditItem("AbsoluteTabTextPadding","����Padding",szSetName);
	System:insertEditItem("RelativeTabTextPadding","���Padding",szSetName);
		
end


--��ѡ��һ�����ڵ�ʱ��,������������,��������ڵĶ�Ӧֵˢ�µ�������,���������Ҫ���������б�
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



--��ȡToolbar����
function getToolBarNameByType(winType)
	if winType == "WoWLook/MultiColumnList" then
		return "�����б��";
	elseif winType == "WoWLook/SoftKeyWindow" then
		return "����̴���";
	elseif winType == "WoWLook/Slider" then
		return "Slider����";
	elseif winType == "WoWLook/PaoPao" then
		return "ð�ݴ���";
	elseif winType == "WoWLook/SelfFitWindow" then
		return "����Ӧ�ı�����";
	elseif winType == "WoWLook/AnimateWindow" then
		return "��������1";
	elseif winType == "WoWLook/Animate" then
		return "��������2";
	elseif winType == "WoWLook/StopWatch" then
		return "����ʱ����";
	elseif winType == "WoWLook/CreatureBoard" then
		return "ͷ����Ϣ��";
	elseif winType == "WoWLook/Minimap" then
		return "MiniMap����";
	elseif winType == "WoWLook/InfoList" then
		return "InfoList����";
	elseif winType == "WoWLook/WorldMap" then
		return "�����ͼ����";
	elseif winType == "WoWLook/SceneMap" then
		return "������ͼ����";
	elseif winType == "WoWLook/SuperProgress" then
		return "��������������";
	elseif winType == "WoWLook/MeshWindow" then
		return "������ʾAvtar����";
	elseif winType == "WoWLook/SimpleText" then
		return "��ͨ�ı�����";
	elseif winType == "WoWLook/Money" then
		return "��Ǯ�ؼ�����";
	elseif winType == "WoWLook/Button_Combo" then
		return "Combo ����";
	elseif winType == "WoWLook/ComboEditbox" then
		return "Combo ����(�ɱ༭)";
	elseif winType == "WoWLook/ComboList" then
		return "Combo ����(���б�)";
	elseif winType == "WoWLook/ComboDropList" then
		return "Combo ����(���б�)";
	elseif winType == "WoWLook/Listbox_Debug" then
		return "�б���ʾ������";
	elseif winType == "WoWLook/Listbox" then
		return "��ͨ�б�򴰿�";
	elseif winType == "WoWLook/SuperTooltip" then
		return "����Tooltip";
	elseif winType == "WoWLook/Tooltip" then
		return "��ͨTooltip";
	elseif winType == "WoWLook/PopupMenu" then
		return "�����˵�����";
	elseif winType == "WoWLook/MenuItem" then
		return "�˵����";
	elseif winType == "WoWLook/SystemButton" then
		return "ϵͳ��ť����";
	elseif winType == "WoWLook/Titlebar_2" then
		return "���ⴰ��2";
	elseif winType == "WoWLook/Titlebar" then
		return "���ⴰ��1";
	elseif winType == "WoWLook/FrameWindow_HelpText" then
		return "��ܴ��ڴ�����";
	elseif winType == "WoWLook/Button_MiniClose" then
		return "MiniClose��ť";
	elseif winType == "WoWLook/HorizontalScrollbar" then
		return "ˮƽ������";
	elseif winType == "WoWLook/VerticalScrollbar" then
		return "��ֱ������";
	elseif winType == "WoWLook/StaticImage" then
		return "��̬ͼ�񴰿�";
	elseif winType == "WoWLook/StaticText3" then
		return "��̬�ı�3";
	elseif winType == "WoWLook/StaticText2" then
		return "��̬�ı�2";
	elseif winType == "WoWLook/StaticText" then
		return "��̬�ı�1";
	elseif winType == "WoWLook/MultiIMEEditBox" then
		return "���������";
	elseif winType == "WoWLook/ChatEditbox_IME" then
		return "�������봰��";
	elseif winType == "WoWLook/IMEEditBox" then
		return "֧�����ĵĵ��������";
	elseif winType == "WoWLook/Editbox_1" then
		return "��ͨ�����1";
	elseif winType == "WoWLook/Editbox_2" then
		return "��ͨ�����2";
	elseif winType == "WoWLook/Editbox_3" then
		return "��ͨ�����3";
	elseif winType == "WoWLook/Editbox_4" then
		return "��ͨ�����4";
	elseif winType == "WoWLook/Editbox_5" then
		return "��ͨ�����5";
	elseif winType == "WoWLook/Editbox_6" then
		return "��ͨ�����6";
	elseif winType == "WoWLook/ActionButton" then
		return "ActionButton����";
	elseif winType == "WoWLook/Button_Text" then
		return "���ı���Button����";
	elseif winType == "WoWLook/Button_1" then
		return "��ť1";
	elseif winType == "WoWLook/Button_2" then
		return "��ť2";
	elseif winType == "WoWLook/Button_3" then
		return "��ť3";
	elseif winType == "WoWLook/Button_4" then
		return "��ť4";
	elseif winType == "WoWLook/Button_5" then
		return "��ť5";
	elseif winType == "WoWLook/Button_6" then
		return "��ť6";
	elseif winType == "WoWLook/Button_7" then
		return "��ť7";
	elseif winType == "WoWLook/Button_8" then
		return "��ť8";
	elseif winType == "WoWLook/ButtonDrop" then
		return "Drop��ť";
	elseif winType == "WoWLook/ButtonSale" then
		return "Sale��ť";
	elseif winType == "WoWLook/FrameWindow" then
		return "��ܴ���";
	end
	return "";
end

