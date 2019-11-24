const { promisify } = require("util");
const { exec } = require("child_process");
const execPromise = promisify(exec);
const fs = require("fs");

const BUMP_TYPES = {
  Major: "major",
  Minor: "minor",
  Patch: "patch"
};

(async () => {
  if (process.argv.length < 3) {
    console.error("ERROR: Please enter a [major/minor/patch] version bump.");
    process.exit(1);
  }

  const bump = process.argv[2].toLowerCase();

  if (
    bump !== BUMP_TYPES.Major &&
    bump !== BUMP_TYPES.Minor &&
    bump !== BUMP_TYPES.Patch
  ) {
    console.error("ERROR: Please enter a [major/minor/patch] version bump.");
    process.exit(1);
  }

  // use native npm version CLI command for npm version bump
  const { stdout } = await execPromise(`npm version ${bump}`, {
    cwd: "./neutralino.js"
  });

  const newVersion = stdout.slice(1).trim();

  // manually bump version number for CPP files
  const cppFilePaths = [
    "./core-linux/src/settings.cpp",
    "./core-macos/src/settings.cpp",
    "./core-windows/src/settings.cpp",
    "./core-windows/bin/version_info"
  ];

  cppFilePaths.forEach(filePath => {
    let contents = fs.readFileSync(filePath, { encoding: "utf-8" });

    if (filePath.includes("settings.cpp")) {
      const oldVersion = contents.match(
        /NL_VERSION='([0-9+]+\.[0-9+]+\.[0-9]+)'/
      )[1];
      const newV = bumpVersion(oldVersion, bump);

      if (newV !== newVersion) {
        console.error(
          `Package versions in files do not match (${newV} and ${newVersion})`
        );
        process.exit(1);
      }

      contents = contents.replace(
        `NL_VERSION='${oldVersion}'`,
        `NL_VERSION='${newVersion}'`
      );
    } else {
      const oldFileVersion = contents.match(
        /"FileVersion", "([0-9+]+\.[0-9+]+\.[0-9]+)"/
      )[1];
      const oldProductVersion = contents.match(
        /"ProductVersion", "([0-9+]+\.[0-9+]+\.[0-9]+)"/
      )[1];

      const newFileVersion = bumpVersion(oldFileVersion, bump);
      const newProductVersion = bumpVersion(oldProductVersion, bump);

      if (newFileVersion !== newVersion || newProductVersion !== newVersion) {
        console.error(
          `Package versions in files do not match (${newFileVersion}/${newProductVersion} and ${newVersion})`
        );
        process.exit(1);
      }

      contents = contents
        .replace(
          `"FileVersion", "${oldFileVersion}"`,
          `"FileVersion", "${newVersion}"`
        )
        .replace(
          `"ProductVersion", "${oldProductVersion}"`,
          `"ProductVersion", "${newVersion}"`
        );
    }

    fs.writeFileSync(filePath, contents);
  });

  // commit and tag
  await execPromise(
    `git add --all && git commit -m "v${newVersion}" && git tag v${newVersion}`
  );
})();

function bumpVersion(oldVersion, bumpType) {
  if (typeof oldVersion !== "string" || typeof bumpType !== "string") {
    return;
  }
  let [majorV, minorV, patchV] = oldVersion.split(".");

  if (bumpType === BUMP_TYPES.Major) {
    majorV++;
    minorV = 0;
    patchV = 0;
  } else if (bumpType === BUMP_TYPES.Minor) {
    minorV++;
    patchV = 0;
  } else if (bumpType === BUMP_TYPES.Patch) {
    patchV++;
  }

  return `${majorV}.${minorV}.${patchV}`;
}
