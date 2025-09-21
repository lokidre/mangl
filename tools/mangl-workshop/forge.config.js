const entryPointRenderer = './src/renderer.ts';
const entryPointIndexHtml = './static/index.html';
const entryPointName = 'main_window';

module.exports = {
  packagerConfig: {},
  //electronRebuildConfig: { ... },
  makers: [
    {
      name: '@electron-forge/maker-squirrel',
      config: {
        name: 'my_new_app',
      },
    },
    {
      name: '@electron-forge/maker-zip',
      platforms: ['darwin'],
    },
    {
      name: '@electron-forge/maker-deb',
      config: {},
    },
    {
      name: '@electron-forge/maker-rpm',
      config: {},
    },
  ],
  //publishers: [ ... ],
  plugins: [
    {
      name: '@electron-forge/plugin-webpack',
      config: {
        mainConfig: './webpack.main.config.js',
        //devContentSecurityPolicy: "connect-src 'self' static://* 'unsafe-eval'",
        devContentSecurityPolicy: "connect-src 'self' * 'unsafe-eval'",
        // devContentSecurityPolicy: "*",
        renderer: {
          config: './webpack.renderer.config.js',
          nodeIntegration: true,
          entryPoints: [
            {
              html: entryPointIndexHtml,
              js: entryPointRenderer,
              name: entryPointName,

              preload: {
                js: './src/preload.ts',
              },
            },
          ],
        },
      },
    },

  ],

  //hooks: { ... },
  //buildIdentifier: "my-build",
};
