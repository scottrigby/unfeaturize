```ruby
.-. .-..-. .-.,---.,---.    .--.  _______ .-. .-.,---.  ,-. _____  ,---.
| | | ||  \| || .-'| .-'   / /\ \|__   __|| | | || .-.\ |(|/___  / | .-'
| | | ||   | || `-.| `-.  / /__\ \ )| |   | | | || `-'/ (_)   / /) | `-.
| | | || |\  || .-'| .-'  |  __  |(_) |   | | | ||   (  | |  / /(_)| .-'
| `-')|| | |)|| |  |  `--.| |  |)|  | |   | `-')|| |\ \ | | / /___ |  `--.
`---(_)/(  (_))\|  /( __.'|_|  (_)  `-'   `---(_)|_| \)\`-'(_____/ /( __.’
      (__)   (__) (__)                               (__)         (__)
```

# Q&A
**Question:** Are Features modules packaged within distributions considered a feature or a bug?

**Answer:** Yes.

# Goals
1. Untether a "feature" module from Features.module.
2. Provide a mechanism to keep original exported component defaults for inserting/reverting, without Features tracking functionality.
3. This will allow new Features to include the default components, without requiring the pains of overriding.
4. Automate as much of this as possible
    - Do not require people implementing distributions to do any manual steps
    - Attempt to limit the manual steps for distribution developers in instituting this change

# Current strategy
1. Keep Features default include file and callback structure the same (so downstream features can contribute fixes upstream more easily)
    - We will need custom callbacks that restore defaults to the DB, because Features revert for CTools-style components (views_view etc) deletes the DB row rather than restoring, unlike other non-destructive components (like field_instance). So what we need is our own variation that always does restore (similar to "Features consolidate" https://drupal.org/node/1014522, or "Features Unlink" https://drupal.org/project/ftools, but without the need to do that manually). These hooks can be added to install hooks and possibly available to Drush for a "hard revert" option independent of Features API.
    - Question: should we rename the files and callbacks, to make it clear the omission of features API and info file lines is not accidental?
2. Reuse Features API functions where possible.
