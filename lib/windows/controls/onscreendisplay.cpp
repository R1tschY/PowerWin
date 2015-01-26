#include "onscreendisplay.h"

namespace Windows {

OnScreenDisplay::OnScreenDisplay() :
  GraphicsControl(
    WS_POPUP,
    WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_NOACTIVATE)
{
  // transparent for clicks, ... : WS_EX_TRANSPARENT
}

void OnScreenDisplay::onPaint(GraphicsContext* context)
{
  //using(LinearGradientBrush b = new LinearGradientBrush(this.Bound, Color.LightBlue, Color.DarkGoldenrod, 45f))
  //e.Graphics.FillRectangle(b, this.Bound);
  //e.Graphics.DrawString("Overide this PerformPaint method...", new Font(FontFamily.GenericSansSerif, 12f, FontStyle.Regular), new SolidBrush(Color.FromArgb(170, Color.Red)) , new PointF(0f, 10f));
}

void OnScreenDisplay::onCreate()
{
  // Make BLACK the transparency color
  SetLayeredWindowAttributes(getNativeHandle(), RGB(0,0,0), 0, LWA_COLORKEY);
}

//void OnScreenDisplay::invalidate()
//{
//  UpdateLayeredWindow();
//}

//void OnScreenDisplay::updateLayeredWindow()
//{
//  Bitmap bitmap1 = new Bitmap(this.Size.Width, this.Size.Height, PixelFormat.Format32bppArgb);
//  using (Graphics graphics1 = Graphics.FromImage(bitmap1))
//  {
//  Rectangle rectangle1;
//  SIZE size1;
//  POINT point1;
//  POINT point2;
//  BLENDFUNCTION blendfunction1;
//  rectangle1 = new Rectangle(0, 0, this.Size.Width, this.Size.Height);
//  this.PerformPaint(new PaintEventArgs(graphics1, rectangle1));
//  IntPtr ptr1 = User32.GetDC(IntPtr.Zero);
//  IntPtr ptr2 = Gdi32.CreateCompatibleDC(ptr1);
//  IntPtr ptr3 = bitmap1.GetHbitmap(Color.FromArgb(0));
//  IntPtr ptr4 = Gdi32.SelectObject(ptr2, ptr3);
//  size1.cx = this.Size.Width;
//  size1.cy = this.Size.Height;
//  point1.x = this.Location.X;
//  point1.y = this.Location.Y;
//  point2.x = 0;
//  point2.y = 0;
//  blendfunction1 = new BLENDFUNCTION();
//  blendfunction1.BlendOp = 0;
//  blendfunction1.BlendFlags = 0;
//  blendfunction1.SourceConstantAlpha = this._alpha;
//  blendfunction1.AlphaFormat = 1;
//  UpdateLayeredWindow(base.Handle, ptr1, point1, size1, ptr2, point2, 0, blendfunction1, 2); //2=ULW_ALPHA
//  Gdi32.SelectObject(ptr2, ptr4);
//  User32.ReleaseDC(IntPtr.Zero, ptr1);
//  Gdi32.DeleteObject(ptr3);
//  Gdi32.DeleteDC(ptr2);
//}
//}

//void OnScreenDisplay::ShowAnimate(AnimateMode mode, unsigned time)
//{
//  if ((mode & AW_BLEND) != 0)
//    AnimateWithBlend(true, time);
//  else
//    AnimateWindow(getNativeHandle(), time, mode);
//}

//void OnScreenDisplay::HideAnimate(AnimateMode mode, uint time)
//{
//  if(base.Handle == IntPtr.Zero)
//    return;

//  mode |= User32.AW_HIDE;
//  if((mode & User32.AW_BLEND) != 0)
//    this.AnimateWithBlend(false, time);
//  else
//    User32.AnimateWindow(getNativeHandle(), time, mode);
//  this.Hide();
//}

//void OnScreenDisplay::AnimateWithBlend(bool show, uint time)
//{
//  byte originalAplha = this._alpha;
//  byte p = (byte)(originalAplha / (time / 10));
//  if(p == 0) p++;
//  if(show)
//  {
//    this._alpha = 0;
//    this.UpdateLayeredWindow();
//    User32.ShowWindow(base.Handle, User32.SW_SHOWNOACTIVATE);
//  }
//  for(byte i = show ? (byte)0 : originalAplha; (show ? i <= originalAplha : i >= (byte)0); i += (byte)(p * (show ? 1 : -1)))
//  {
//    this._alpha = i;
//    this.UpdateLayeredWindow();
//    if((show && i > originalAplha - p) || (!show && i < p))
//      break;
//  }
//  this._alpha = originalAplha;
//  if(show)
//    this.UpdateLayeredWindow();
//}


} // namespace Windows
