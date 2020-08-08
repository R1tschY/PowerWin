use lazy_static::lazy_static;
use std::borrow::Cow;
use std::collections::HashMap;

lazy_static! {
    static ref MESSAGES: HashMap<u32, &'static str> = fill_messages();
}

pub fn get_message_name(id: u32) -> Cow<'static, str> {
    match MESSAGES.get(&id) {
        Some(name) => (*name).into(),
        None => format!("WM({})", id).into(),
    }
}

fn fill_messages() -> HashMap<u32, &'static str> {
    let mut messages: HashMap<u32, &'static str> = HashMap::new();
    messages.insert(0, "WM_NULL");
    messages.insert(1, "WM_CREATE");
    messages.insert(2, "WM_DESTROY");
    messages.insert(3, "WM_MOVE");
    messages.insert(5, "WM_SIZE");
    messages.insert(6, "WM_ACTIVATE");
    messages.insert(7, "WM_SETFOCUS");
    messages.insert(8, "WM_KILLFOCUS");
    messages.insert(10, "WM_ENABLE");
    messages.insert(11, "WM_SETREDRAW");
    messages.insert(12, "WM_SETTEXT");
    messages.insert(13, "WM_GETTEXT");
    messages.insert(14, "WM_GETTEXTLENGTH");
    messages.insert(15, "WM_PAINT");
    messages.insert(16, "WM_CLOSE");
    messages.insert(17, "WM_QUERYENDSESSION");
    messages.insert(18, "WM_QUIT");
    messages.insert(19, "WM_QUERYOPEN");
    messages.insert(20, "WM_ERASEBKGND");
    messages.insert(21, "WM_SYSCOLORCHANGE");
    messages.insert(22, "WM_ENDSESSION");
    messages.insert(24, "WM_SHOWWINDOW");
    messages.insert(25, "WM_CTLCOLOR");
    messages.insert(26, "WM_WININICHANGE");
    messages.insert(27, "WM_DEVMODECHANGE");
    messages.insert(28, "WM_ACTIVATEAPP");
    messages.insert(29, "WM_FONTCHANGE");
    messages.insert(30, "WM_TIMECHANGE");
    messages.insert(31, "WM_CANCELMODE");
    messages.insert(32, "WM_SETCURSOR");
    messages.insert(33, "WM_MOUSEACTIVATE");
    messages.insert(34, "WM_CHILDACTIVATE");
    messages.insert(35, "WM_QUEUESYNC");
    messages.insert(36, "WM_GETMINMAXINFO");
    messages.insert(38, "WM_PAINTICON");
    messages.insert(39, "WM_ICONERASEBKGND");
    messages.insert(40, "WM_NEXTDLGCTL");
    messages.insert(42, "WM_SPOOLERSTATUS");
    messages.insert(43, "WM_DRAWITEM");
    messages.insert(44, "WM_MEASUREITEM");
    messages.insert(45, "WM_DELETEITEM");
    messages.insert(46, "WM_VKEYTOITEM");
    messages.insert(47, "WM_CHARTOITEM");
    messages.insert(48, "WM_SETFONT");
    messages.insert(49, "WM_GETFONT");
    messages.insert(50, "WM_SETHOTKEY");
    messages.insert(51, "WM_GETHOTKEY");
    messages.insert(55, "WM_QUERYDRAGICON");
    messages.insert(57, "WM_COMPAREITEM");
    messages.insert(61, "WM_GETOBJECT");
    messages.insert(65, "WM_COMPACTING");
    messages.insert(68, "WM_COMMNOTIFY");
    messages.insert(70, "WM_WINDOWPOSCHANGING");
    messages.insert(71, "WM_WINDOWPOSCHANGED");
    messages.insert(72, "WM_POWER");
    messages.insert(73, "WM_COPYGLOBALDATA");
    messages.insert(74, "WM_COPYDATA");
    messages.insert(75, "WM_CANCELJOURNAL");
    messages.insert(78, "WM_NOTIFY");
    messages.insert(80, "WM_INPUTLANGCHANGEREQUEST");
    messages.insert(81, "WM_INPUTLANGCHANGE");
    messages.insert(82, "WM_TCARD");
    messages.insert(83, "WM_HELP");
    messages.insert(84, "WM_USERCHANGED");
    messages.insert(85, "WM_NOTIFYFORMAT");
    messages.insert(123, "WM_CONTEXTMENU");
    messages.insert(124, "WM_STYLECHANGING");
    messages.insert(125, "WM_STYLECHANGED");
    messages.insert(126, "WM_DISPLAYCHANGE");
    messages.insert(127, "WM_GETICON");
    messages.insert(128, "WM_SETICON");
    messages.insert(129, "WM_NCCREATE");
    messages.insert(130, "WM_NCDESTROY");
    messages.insert(131, "WM_NCCALCSIZE");
    messages.insert(132, "WM_NCHITTEST");
    messages.insert(133, "WM_NCPAINT");
    messages.insert(134, "WM_NCACTIVATE");
    messages.insert(135, "WM_GETDLGCODE");
    messages.insert(136, "WM_SYNCPAINT");
    messages.insert(160, "WM_NCMOUSEMOVE");
    messages.insert(161, "WM_NCLBUTTONDOWN");
    messages.insert(162, "WM_NCLBUTTONUP");
    messages.insert(163, "WM_NCLBUTTONDBLCLK");
    messages.insert(164, "WM_NCRBUTTONDOWN");
    messages.insert(165, "WM_NCRBUTTONUP");
    messages.insert(166, "WM_NCRBUTTONDBLCLK");
    messages.insert(167, "WM_NCMBUTTONDOWN");
    messages.insert(168, "WM_NCMBUTTONUP");
    messages.insert(169, "WM_NCMBUTTONDBLCLK");
    messages.insert(171, "WM_NCXBUTTONDOWN");
    messages.insert(172, "WM_NCXBUTTONUP");
    messages.insert(173, "WM_NCXBUTTONDBLCLK");
    messages.insert(176, "EM_GETSEL");
    messages.insert(177, "EM_SETSEL");
    messages.insert(178, "EM_GETRECT");
    messages.insert(179, "EM_SETRECT");
    messages.insert(180, "EM_SETRECTNP");
    messages.insert(181, "EM_SCROLL");
    messages.insert(182, "EM_LINESCROLL");
    messages.insert(183, "EM_SCROLLCARET");
    messages.insert(185, "EM_GETMODIFY");
    messages.insert(187, "EM_SETMODIFY");
    messages.insert(188, "EM_GETLINECOUNT");
    messages.insert(189, "EM_LINEINDEX");
    messages.insert(190, "EM_SETHANDLE");
    messages.insert(191, "EM_GETHANDLE");
    messages.insert(192, "EM_GETTHUMB");
    messages.insert(193, "EM_LINELENGTH");
    messages.insert(194, "EM_REPLACESEL");
    messages.insert(195, "EM_SETFONT");
    messages.insert(196, "EM_GETLINE");
    messages.insert(197, "EM_LIMITTEXT");
    messages.insert(197, "EM_SETLIMITTEXT");
    messages.insert(198, "EM_CANUNDO");
    messages.insert(199, "EM_UNDO");
    messages.insert(200, "EM_FMTLINES");
    messages.insert(201, "EM_LINEFROMCHAR");
    messages.insert(202, "EM_SETWORDBREAK");
    messages.insert(203, "EM_SETTABSTOPS");
    messages.insert(204, "EM_SETPASSWORDCHAR");
    messages.insert(205, "EM_EMPTYUNDOBUFFER");
    messages.insert(206, "EM_GETFIRSTVISIBLELINE");
    messages.insert(207, "EM_SETREADONLY");
    messages.insert(209, "EM_SETWORDBREAKPROC");
    messages.insert(209, "EM_GETWORDBREAKPROC");
    messages.insert(210, "EM_GETPASSWORDCHAR");
    messages.insert(211, "EM_SETMARGINS");
    messages.insert(212, "EM_GETMARGINS");
    messages.insert(213, "EM_GETLIMITTEXT");
    messages.insert(214, "EM_POSFROMCHAR");
    messages.insert(215, "EM_CHARFROMPOS");
    messages.insert(216, "EM_SETIMESTATUS");
    messages.insert(217, "EM_GETIMESTATUS");
    messages.insert(224, "SBM_SETPOS");
    messages.insert(225, "SBM_GETPOS");
    messages.insert(226, "SBM_SETRANGE");
    messages.insert(227, "SBM_GETRANGE");
    messages.insert(228, "SBM_ENABLE_ARROWS");
    messages.insert(230, "SBM_SETRANGEREDRAW");
    messages.insert(233, "SBM_SETSCROLLINFO");
    messages.insert(234, "SBM_GETSCROLLINFO");
    messages.insert(235, "SBM_GETSCROLLBARINFO");
    messages.insert(240, "BM_GETCHECK");
    messages.insert(241, "BM_SETCHECK");
    messages.insert(242, "BM_GETSTATE");
    messages.insert(243, "BM_SETSTATE");
    messages.insert(244, "BM_SETSTYLE");
    messages.insert(245, "BM_CLICK");
    messages.insert(246, "BM_GETIMAGE");
    messages.insert(247, "BM_SETIMAGE");
    messages.insert(248, "BM_SETDONTCLICK");
    messages.insert(255, "WM_INPUT");
    messages.insert(256, "WM_KEYDOWN");
    messages.insert(256, "WM_KEYFIRST");
    messages.insert(257, "WM_KEYUP");
    messages.insert(258, "WM_CHAR");
    messages.insert(259, "WM_DEADCHAR");
    messages.insert(260, "WM_SYSKEYDOWN");
    messages.insert(261, "WM_SYSKEYUP");
    messages.insert(262, "WM_SYSCHAR");
    messages.insert(263, "WM_SYSDEADCHAR");
    messages.insert(265, "WM_UNICHAR");
    messages.insert(265, "WM_WNT_CONVERTREQUESTEX");
    messages.insert(266, "WM_CONVERTREQUEST");
    messages.insert(267, "WM_CONVERTRESULT");
    messages.insert(268, "WM_INTERIM");
    messages.insert(269, "WM_IME_STARTCOMPOSITION");
    messages.insert(270, "WM_IME_ENDCOMPOSITION");
    messages.insert(271, "WM_IME_COMPOSITION");
    messages.insert(271, "WM_IME_KEYLAST");
    messages.insert(272, "WM_INITDIALOG");
    messages.insert(273, "WM_COMMAND");
    messages.insert(274, "WM_SYSCOMMAND");
    messages.insert(275, "WM_TIMER");
    messages.insert(276, "WM_HSCROLL");
    messages.insert(277, "WM_VSCROLL");
    messages.insert(278, "WM_INITMENU");
    messages.insert(279, "WM_INITMENUPOPUP");
    messages.insert(280, "WM_SYSTIMER");
    messages.insert(287, "WM_MENUSELECT");
    messages.insert(288, "WM_MENUCHAR");
    messages.insert(289, "WM_ENTERIDLE");
    messages.insert(290, "WM_MENURBUTTONUP");
    messages.insert(291, "WM_MENUDRAG");
    messages.insert(292, "WM_MENUGETOBJECT");
    messages.insert(293, "WM_UNINITMENUPOPUP");
    messages.insert(294, "WM_MENUCOMMAND");
    messages.insert(295, "WM_CHANGEUISTATE");
    messages.insert(296, "WM_UPDATEUISTATE");
    messages.insert(297, "WM_QUERYUISTATE");
    messages.insert(306, "WM_CTLCOLORMSGBOX");
    messages.insert(307, "WM_CTLCOLOREDIT");
    messages.insert(308, "WM_CTLCOLORLISTBOX");
    messages.insert(309, "WM_CTLCOLORBTN");
    messages.insert(310, "WM_CTLCOLORDLG");
    messages.insert(311, "WM_CTLCOLORSCROLLBAR");
    messages.insert(312, "WM_CTLCOLORSTATIC");
    messages.insert(512, "WM_MOUSEFIRST");
    messages.insert(512, "WM_MOUSEMOVE");
    messages.insert(513, "WM_LBUTTONDOWN");
    messages.insert(514, "WM_LBUTTONUP");
    messages.insert(515, "WM_LBUTTONDBLCLK");
    messages.insert(516, "WM_RBUTTONDOWN");
    messages.insert(517, "WM_RBUTTONUP");
    messages.insert(518, "WM_RBUTTONDBLCLK");
    messages.insert(519, "WM_MBUTTONDOWN");
    messages.insert(520, "WM_MBUTTONUP");
    messages.insert(521, "WM_MBUTTONDBLCLK");
    messages.insert(521, "WM_MOUSELAST");
    messages.insert(522, "WM_MOUSEWHEEL");
    messages.insert(523, "WM_XBUTTONDOWN");
    messages.insert(524, "WM_XBUTTONUP");
    messages.insert(525, "WM_XBUTTONDBLCLK");
    messages.insert(526, "WM_MOUSEHWHEEL");
    messages.insert(528, "WM_PARENTNOTIFY");
    messages.insert(529, "WM_ENTERMENULOOP");
    messages.insert(530, "WM_EXITMENULOOP");
    messages.insert(531, "WM_NEXTMENU");
    messages.insert(532, "WM_SIZING");
    messages.insert(533, "WM_CAPTURECHANGED");
    messages.insert(534, "WM_MOVING");
    messages.insert(536, "WM_POWERBROADCAST");
    messages.insert(537, "WM_DEVICECHANGE");
    messages.insert(544, "WM_MDICREATE");
    messages.insert(545, "WM_MDIDESTROY");
    messages.insert(546, "WM_MDIACTIVATE");
    messages.insert(547, "WM_MDIRESTORE");
    messages.insert(548, "WM_MDINEXT");
    messages.insert(549, "WM_MDIMAXIMIZE");
    messages.insert(550, "WM_MDITILE");
    messages.insert(551, "WM_MDICASCADE");
    messages.insert(552, "WM_MDIICONARRANGE");
    messages.insert(553, "WM_MDIGETACTIVE");
    messages.insert(560, "WM_MDISETMENU");
    messages.insert(561, "WM_ENTERSIZEMOVE");
    messages.insert(562, "WM_EXITSIZEMOVE");
    messages.insert(563, "WM_DROPFILES");
    messages.insert(564, "WM_MDIREFRESHMENU");
    messages.insert(640, "WM_IME_REPORT");
    messages.insert(641, "WM_IME_SETCONTEXT");
    messages.insert(642, "WM_IME_NOTIFY");
    messages.insert(643, "WM_IME_CONTROL");
    messages.insert(644, "WM_IME_COMPOSITIONFULL");
    messages.insert(645, "WM_IME_SELECT");
    messages.insert(646, "WM_IME_CHAR");
    messages.insert(648, "WM_IME_REQUEST");
    messages.insert(656, "WM_IMEKEYDOWN");
    messages.insert(656, "WM_IME_KEYDOWN");
    messages.insert(657, "WM_IMEKEYUP");
    messages.insert(657, "WM_IME_KEYUP");
    messages.insert(672, "WM_NCMOUSEHOVER");
    messages.insert(673, "WM_MOUSEHOVER");
    messages.insert(674, "WM_NCMOUSELEAVE");
    messages.insert(675, "WM_MOUSELEAVE");
    messages.insert(768, "WM_CUT");
    messages.insert(769, "WM_COPY");
    messages.insert(770, "WM_PASTE");
    messages.insert(771, "WM_CLEAR");
    messages.insert(772, "WM_UNDO");
    messages.insert(773, "WM_RENDERFORMAT");
    messages.insert(774, "WM_RENDERALLFORMATS");
    messages.insert(775, "WM_DESTROYCLIPBOARD");
    messages.insert(776, "WM_DRAWCLIPBOARD");
    messages.insert(777, "WM_PAINTCLIPBOARD");
    messages.insert(778, "WM_VSCROLLCLIPBOARD");
    messages.insert(779, "WM_SIZECLIPBOARD");
    messages.insert(780, "WM_ASKCBFORMATNAME");
    messages.insert(781, "WM_CHANGECBCHAIN");
    messages.insert(782, "WM_HSCROLLCLIPBOARD");
    messages.insert(783, "WM_QUERYNEWPALETTE");
    messages.insert(784, "WM_PALETTEISCHANGING");
    messages.insert(785, "WM_PALETTECHANGED");
    messages.insert(786, "WM_HOTKEY");
    messages.insert(791, "WM_PRINT");
    messages.insert(792, "WM_PRINTCLIENT");
    messages.insert(793, "WM_APPCOMMAND");
    messages.insert(856, "WM_HANDHELDFIRST");
    messages.insert(863, "WM_HANDHELDLAST");
    messages.insert(864, "WM_AFXFIRST");
    messages.insert(895, "WM_AFXLAST");
    messages.insert(896, "WM_PENWINFIRST");
    messages.insert(897, "WM_RCRESULT");
    messages.insert(898, "WM_HOOKRCRESULT");
    messages.insert(899, "WM_GLOBALRCCHANGE");
    messages.insert(899, "WM_PENMISCINFO");
    messages.insert(900, "WM_SKB");
    messages.insert(901, "WM_HEDITCTL");
    messages.insert(901, "WM_PENCTL");
    messages.insert(902, "WM_PENMISC");
    messages.insert(903, "WM_CTLINIT");
    messages.insert(904, "WM_PENEVENT");
    messages.insert(911, "WM_PENWINLAST");
    messages
}