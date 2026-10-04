# Issue tracker

Issues live in GitHub Issues on `Northeastern-Electric-Racing/Nero-2.0`. File them from the templates in .github/ISSUE_TEMPLATE; each field's description says what goes in it.

## Pipeline

idea → spikes → epic → dev work

| Ticket | What it is | Issue type | Parent |
|---|---|---|---|
| Idea | A feature or direction worth exploring. Short: the idea, its rationale, and the approaches to explore. | Feature | none |
| Spike | One approach to an idea, usually a separate implementation of it, or a small standalone improvement. States what is explored and the hypothesis; findings go in comments and draft PRs, not the ticket. | Task | the idea, if any |
| Epic | A settled idea: the approach is decided. Includes a diagram of the change. | Feature | none; links the idea in "From idea" |
| Dev work | Any implementation work: features, bugs, and tasks share one template. | Feature, Bug, or Task | the epic, if any |

When an idea is settled, open an epic that links back to it and close the idea as completed.

Link children with GitHub sub-issues: spikes under their idea, dev work under its epic.

## Labels

Every issue gets one label from each of the first two groups and at least one area label.

| Group | Labels |
|---|---|
| Written by | `by: human`, `by: ai`, `by: ai-assisted` |
| Category | `idea`, `spike`, `epic`, `dev work` (the template applies it) |
| Area | `view`, `controller`, `model`, `devops` (any combination) |

Bug, feature, and task are GitHub issue types, not labels.

## Writing tickets

- **Title:** concise and imperative ("Add a lap timer to the pit screen"), no prefixes.
- **Backticks:** at most three inline backtick usages per body. Reference files and identifiers in plain text. Diagram and context-transfer code blocks don't count.
- **Context transfer:** an optional code block on spikes, epics, and dev work for handing off to a person or agent. Start it with a short summary, then the raw context.
- **Assignment:** assign yourself when you file.
