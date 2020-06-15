use std::env;
use std::io;

const MANIFEST_TEMPLATE: &'static str = r#"<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" xmlns:asmv3="urn:schemas-microsoft-com:asm.v3" >

  <!-- app -->
  <assemblyIdentity
    version="@CARGO_PKG_VERSION@"
    type="Win32"
    processorArchitecture="x86"
    name="R1tschY.PowerWin"/>

  <!-- no admin privileges needed -->
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v2">
    <security>
      <requestedPrivileges>
        <requestedExecutionLevel level="asInvoker" uiAccess="false"/>
      </requestedPrivileges>
    </security>
  </trustInfo>

  <!-- use CC v6.0 -->
  <dependency>
    <dependentAssembly>
      <assemblyIdentity type="Win32" name="Microsoft.Windows.Common-Controls" version="6.0.0.0"
                        processorArchitecture="*" publicKeyToken="6595b64144ccf1df" language="*"/>
    </dependentAssembly>
  </dependency>

  <!-- DPI aware -->
  <asmv3:application>
    <asmv3:windowsSettings xmlns="http://schemas.microsoft.com/SMI/2005/WindowsSettings">
      <dpiAware>true</dpiAware>
    </asmv3:windowsSettings>
  </asmv3:application>
</assembly>"#;

fn to_windows_version(s: &str) -> String {
    match s.matches('.').count() {
        0 => format!("{}.0.0.0", s),
        1 => format!("{}.0.0", s),
        2 => format!("{}.0", s),
        3 => String::from(s),
        _ => panic!("invalid package version"),
    }
}

fn main() -> io::Result<()> {
    // create version info resource and compile
    let versioninfo = format!("{}/versioninfo.rc", env::var("OUT_DIR").unwrap());
    winres::WindowsResource::new()
        .set("LegalCopyright", "Copyright © 2019 Richard Liebscher")
        .set_manifest(
            &MANIFEST_TEMPLATE
                .replace(
                    "@CARGO_PKG_VERSION@",
                    &to_windows_version(env!("CARGO_PKG_VERSION")),
                )
                .replace("\n", " "),
        ) // FIXME: winres bug
        .write_resource_file(&versioninfo)?;
    embed_resource::compile(&versioninfo);

    // compile app resources
    embed_resource::compile("src/resources.rc");

    Ok(())
}
