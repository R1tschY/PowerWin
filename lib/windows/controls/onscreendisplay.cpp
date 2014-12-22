#include "onscreendisplay.h"

namespace Windows {

OnScreenDisplay::OnScreenDisplay() :
  Control(
    WS_POPUP,
    WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT)
{
}

virtual void OnScreenDisplay::PerformPaint(PaintEventArgs e)
{
  //using(LinearGradientBrush b = new LinearGradientBrush(this.Bound, Color.LightBlue, Color.DarkGoldenrod, 45f))
  //e.Graphics.FillRectangle(b, this.Bound);
  //e.Graphics.DrawString("Overide this PerformPaint method...", new Font(FontFamily.GenericSansSerif, 12f, FontStyle.Regular), new SolidBrush(Color.FromArgb(170, Color.Red)) , new PointF(0f, 10f));
}

void OnScreenDisplay::Invalidate()
{
  UpdateLayeredWindow();
}

void OnScreenDisplay::UpdateLayeredWindow()
{
  Bitmap bitmap1 = new Bitmap(this.Size.Width, this.Size.Height, PixelFormat.Format32bppArgb);
  using (Graphics graphics1 = Graphics.FromImage(bitmap1))
  {
  Rectangle rectangle1;
  SIZE size1;
  POINT point1;
  POINT point2;
  BLENDFUNCTION blendfunction1;
  rectangle1 = new Rectangle(0, 0, this.Size.Width, this.Size.Height);
  this.PerformPaint(new PaintEventArgs(graphics1, rectangle1));
  IntPtr ptr1 = User32.GetDC(IntPtr.Zero);
  IntPtr ptr2 = Gdi32.CreateCompatibleDC(ptr1);
  IntPtr ptr3 = bitmap1.GetHbitmap(Color.FromArgb(0));
  IntPtr ptr4 = Gdi32.SelectObject(ptr2, ptr3);
  size1.cx = this.Size.Width;
  size1.cy = this.Size.Height;
  point1.x = this.Location.X;
  point1.y = this.Location.Y;
  point2.x = 0;
  point2.y = 0;
  blendfunction1 = new BLENDFUNCTION();
  blendfunction1.BlendOp = 0;
  blendfunction1.BlendFlags = 0;
  blendfunction1.SourceConstantAlpha = this._alpha;
  blendfunction1.AlphaFormat = 1;
  UpdateLayeredWindow(base.Handle, ptr1, point1, size1, ptr2, point2, 0, blendfunction1, 2); //2=ULW_ALPHA
  Gdi32.SelectObject(ptr2, ptr4);
  User32.ReleaseDC(IntPtr.Zero, ptr1);
  Gdi32.DeleteObject(ptr3);
  Gdi32.DeleteDC(ptr2);
}
}

void OnScreenDisplay::ShowAnimate(AnimateMode mode, unsigned time)
{
  if ((mode & AW_BLEND) != 0)
    AnimateWithBlend(true, time);
  else
    AnimateWindow(getNativeHandle(), time, mode);
}

void OnScreenDisplay::Hide()
{
  if (base.Handle == IntPtr.Zero)
    return;
  ShowWindow(base.Handle, User32.SW_HIDE);
  this.DestroyHandle();
}

void OnScreenDisplay::HideAnimate(AnimateMode mode, uint time)
{
  if(base.Handle == IntPtr.Zero)
    return;

  mode |= User32.AW_HIDE;
  if((mode & User32.AW_BLEND) != 0)
    this.AnimateWithBlend(false, time);
  else
    User32.AnimateWindow(getNativeHandle(), time, mode);
  this.Hide();
}

void OnScreenDisplay::Close()
{
  this.Hide();
  this.Dispose();
}

void OnScreenDisplay::AnimateWithBlend(bool show, uint time)
{
  byte originalAplha = this._alpha;
  byte p = (byte)(originalAplha / (time / 10));
  if(p == 0) p++;
  if(show)
  {
    this._alpha = 0;
    this.UpdateLayeredWindow();
    User32.ShowWindow(base.Handle, User32.SW_SHOWNOACTIVATE);
  }
  for(byte i = show ? (byte)0 : originalAplha; (show ? i <= originalAplha : i >= (byte)0); i += (byte)(p * (show ? 1 : -1)))
  {
    this._alpha = i;
    this.UpdateLayeredWindow();
    if((show && i > originalAplha - p) || (!show && i < p))
      break;
  }
  this._alpha = originalAplha;
  if(show)
    this.UpdateLayeredWindow();
}

void OnScreenDisplay::PerformWmPaint_WmPrintClient(Message m, bool isPaintMessage)
{
  PAINTSTRUCT paintstruct1;
  RECT rect1;
  Rectangle rectangle1;
  paintstruct1 = new PAINTSTRUCT();
  IntPtr ptr1 = (isPaintMessage ? User32.BeginPaint(m.HWnd, ref paintstruct1) : m.WParam);
  rect1 = new RECT();
  User32.GetWindowRect(base.Handle, ref rect1);
  rectangle1 = new Rectangle(0, 0, rect1.right - rect1.left, rect1.bottom - rect1.top);
  using (Graphics graphics1 = Graphics.FromHdc(ptr1))
  {
  Bitmap bitmap1 = new Bitmap(rectangle1.Width, rectangle1.Height);
  using (Graphics graphics2 = Graphics.FromImage(bitmap1))
  this.PerformPaint(new PaintEventArgs(graphics2, rectangle1));
  graphics1.DrawImageUnscaled(bitmap1, 0, 0);
}
if(isPaintMessage)
User32.EndPaint(m.HWnd, ref paintstruct1);
}

void OnScreenDisplay::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg) {
  case WM_PAINT:
    PerformWmPaint_WmPrintClient(ref m, true);
    return 0;

  case WM_PRINTCLIENT:
    PerformWmPaint_WmPrintClient(ref m, false);
    return 0;

  default:
    return Control::onMessage(msg, wparam, lparam);
  }
}

void OnScreenDisplay::SetBoundsCore(int x, int y, int width, int height)
{
  if (((this.X != x) || (this.Y != y)) || ((this.Width != width) || (this.Height != height)))
  {
    if (base.Handle != IntPtr.Zero)
    {
      int num1 = 20;
      if ((this.X == x) && (this.Y == y))
      {
        num1 |= 2;
      }
      if ((this.Width == width) && (this.Height == height))
      {
        num1 |= 1;
      }
      SetWindowPos(base.Handle, IntPtr.Zero, x, y, width, height, (uint)num1);
    }
    else
    {
      this.Location = new Point(x, y);
      this.Size = new Size(width, height);
    }
  }
}

} // namespace Windows
